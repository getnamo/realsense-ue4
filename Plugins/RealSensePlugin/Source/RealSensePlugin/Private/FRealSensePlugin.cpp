#include "RealSensePluginPrivatePCH.h"

#include "IRealSensePlugin.h"
#include "FRealSensePlugin.h"
#include "SlateBasics.h"

#include <windows.h>
#include <wchar.h>
#include "pxcsensemanager.h"
#include "pxchandconfiguration.h"


#include "RealSenseJoint.h"
#include "RealSenseHand.h"
#include "RealSenseInterface.h"

#define PLUGIN_VERSION "0.1.2"


IMPLEMENT_MODULE(FRealSensePlugin, RealSensePlugin)

class DataCollector{
public:
	DataCollector()
	{
		// create the PXCSenseManager
		psm = PXCSenseManager::CreateInstance();
		if (!psm)
		{
			UE_LOG(LogClass, Error, TEXT("RealSense failed to create instance, Unable to create the PXCSenseManager."));
			return;
		}
		// Retrieve the underlying session created by the PXCSenseManager.
		// The returned instance is an PXCSenseManager internally managed object.
		session = psm->QuerySession();
		if (session == NULL)
		{
			UE_LOG(LogClass, Error, TEXT("RealSense failed to create instance, Session not created by PXCSenseManager"));
			return;
		}

		//Set the version, log it for debugging.
		ver = session->QueryVersion();
		UE_LOG(LogClass, Log, TEXT("Plugin v%s, Intel RSSDK Version %d.%d"), TEXT(PLUGIN_VERSION), ver.major, ver.minor);

		//log all available modules that are automatically loaded with the RSSDK
		for (int i = 0;; i++) {
			PXCSession::ImplDesc desc;
			if (session->QueryImpl(0, i, &desc) < PXC_STATUS_NO_ERROR) break;
			// Print the module friendly name and iuid (interface unique ID)
			UE_LOG(LogClass, Log, TEXT("Module[%d]: %s\n"), i, desc.friendlyName);
			UE_LOG(LogClass, Log, TEXT(" iuid = %x\n"), desc.iuid);
		}

		//Temp Vars
		tempJoint = NewObject < URealSenseJoint >();
		tempHand = NewObject < URealSenseHand >();
	}
	~DataCollector()
	{
		psm->Release();
	}

	PXCSenseManager *psm;
	
	PXCHandModule *handModule;
	PXCHandData* handData;

	PXCFaceModule* faceModule;
	PXCFaceData* faceData;

	PXCSession *session;
	PXCSession::ImplVersion ver;

	URealSenseJoint* tempJoint;
	URealSenseHand* tempHand;

	PXCHandConfiguration* handConfig = NULL;

	UObject* interfaceDelegate;
};

//Init and Runtime
void FRealSensePlugin::StartupModule()
{
	//Make a new collector
	collector = new DataCollector;

	//Enable hand Tracking
	EnableHandDetection();
	//EnableGestureDetection(true);	//should be called instead
	
	//EnableFaceDetection();
}

void FRealSensePlugin::ShutdownModule()
{
	delete collector;
}


//for now this enables all gestures, future version will have control over this
void FRealSensePlugin::EnableGestureDetection(bool enableAll)
{
	PXCHandConfiguration* config = collector->handConfig;

	//Null check, hand has to be enabled before we can enable gestures
	if (config == NULL)
		return;

	//enable gestures
	config->LoadGesturePack(L"navigation");

	//enable all gestures
	if (enableAll)
		config->EnableAllGestures();
	else
		config->DisableAllGestures();

	//single gesture enabling example
	//config->EnableGesture("tap");	

	config->ApplyChanges();
	config->Update();
}

void FRealSensePlugin::EnableHandDetection()
{
	//local pointer
	PXCSenseManager* pp = collector->psm;

	/* Set Module */
	pxcStatus status =  pp->EnableHand(0);
	collector->handModule = pp->QueryHand();

	PXCHandModule* handModule = collector->handModule;

	if (handModule == NULL || status != pxcStatus::PXC_STATUS_NO_ERROR)
	{
		UE_LOG(LogClass, Log, TEXT("Failed to pair the gesture module with I/O"));
		return;
	}

	if (pp->Init() >= PXC_STATUS_NO_ERROR)
	{
		collector->handData = collector->handModule->CreateOutput();
	}

	// Hand Module Configuration
	PXCHandConfiguration* config = handModule->CreateActiveConfiguration();
	config->EnableNormalizedJoints(true);
	if (true) config->SetTrackingMode(PXCHandData::TRACKING_MODE_FULL_HAND);
	config->EnableAllAlerts();
	//config->EnableSegmentationImage(true);

	config->ApplyChanges();
	config->Update();

	collector->handConfig = config;

	//PXCHandData* outputData = handAnalyzer->CreateOutput();
	//outputData->Update();
}

void FRealSensePlugin::EnableFaceDetection()
{
	//local pointer
	PXCSenseManager* pp = collector->psm;

	pxcStatus status = pp->EnableFace();

	collector->faceModule = pp->QueryFace();

	PXCFaceModule* faceModule = collector->faceModule;

	if (faceModule == NULL || status != pxcStatus::PXC_STATUS_NO_ERROR)
	{
		UE_LOG(LogClass, Log, TEXT("Failed to pair the gesture module with I/O"));
		return;
	}

	if (pp->Init() >= PXC_STATUS_NO_ERROR)
	{
		collector->faceData = collector->faceModule->CreateOutput();
	}

}

void FRealSensePlugin::SetInterfaceDelegate(UObject* object)
{
	if (object->GetClass()->ImplementsInterface(URealSenseInterface::StaticClass()))
	{
		collector->interfaceDelegate = object;
	}
}

//Delegate methods
void FRealSensePlugin::RealSenseTick(float DeltaTime)
{
	//UE_LOG(LogClass, Log, TEXT("Plugin::RealSenseTick"));
	//local pointer
	PXCSenseManager* pp = collector->psm;

	//Grab Frame
	pxcStatus sts = pp->AcquireFrame(true);

	//Check Frame status
	if (sts < PXC_STATUS_NO_ERROR)
		return;

	//log the hand
	PXCHandData* handData = collector->handData; //handModule->CreateOutput();
	if (handData == NULL)
	{
		UE_LOG(LogClass, Warning, TEXT("Failed to bind hand data"));
		return;
	}

	handData->Update();

	PXCHandData::JointData nodes[2][PXCHandData::NUMBER_OF_JOINTS] = {};
	
	PXCHandData::ExtremityData extremitiesPointsNodes[2][PXCHandData::NUMBER_OF_EXTREMITIES] = {};

	//Iterate hands
	for (pxcI32 i = 0; i < handData->QueryNumberOfHands(); i++)
	{
		//get hand information
		PXCHandData::IHand* hand;

		if (handData->QueryHandData(PXCHandData::AccessOrderType::ACCESS_ORDER_BY_TIME, i, hand) == PXC_STATUS_NO_ERROR)
		{
			//set and emit hand events
			collector->tempHand->setFromRealSenseHand(hand);
			IRealSenseInterface::Execute_HandMoved(collector->interfaceDelegate, collector->tempHand);

			//Iterate Joints
			PXCHandData::JointData jointData;
			for (int j = 0; j < PXCHandData::NUMBER_OF_JOINTS; j++)
			{
				hand->QueryTrackedJoint((PXCHandData::JointType)j, jointData);

				nodes[i][j] = jointData;

				//set our pointer
				collector->tempJoint->setFromRealSenseJoint(&jointData);

				//emit it
				IRealSenseInterface::Execute_JointMoved(collector->interfaceDelegate, collector->tempJoint);

				//FVector vect = FVector(jointData.positionWorld.x, jointData.positionWorld.y, jointData.positionWorld.z);
				//UE_LOG(LogClass, Log, TEXT("Joint: %s"), *vect.ToString());
			}

			//emit hand gesture events
			PXCHandData::GestureData gestureData;

			//spread fingers
			if (handData->IsGestureFired(L"spreadfingers", gestureData))
			{
				IRealSenseInterface::Execute_SpreadFingersGestureDetected(collector->interfaceDelegate, collector->tempHand);
			}

			//fist
			if (handData->IsGestureFired(L"fist", gestureData))
			{
				IRealSenseInterface::Execute_FistGestureDetected(collector->interfaceDelegate, collector->tempHand);
			}

			//tap
			if (handData->IsGestureFired(L"tap", gestureData))
			{
				IRealSenseInterface::Execute_TapGestureDetected(collector->interfaceDelegate, collector->tempHand);
			}

			//thumb_down
			if (handData->IsGestureFired(L"thumb_down", gestureData))
			{
				IRealSenseInterface::Execute_ThumbDownGestureDetected(collector->interfaceDelegate, collector->tempHand);
			}

			//thumb_up
			if (handData->IsGestureFired(L"thumb_up", gestureData))
			{
				IRealSenseInterface::Execute_ThumbUpGestureDetected(collector->interfaceDelegate, collector->tempHand);
			}

			//two_fingers_pinch_open 
			if (handData->IsGestureFired(L"two_fingers_pinch_open", gestureData))
			{
				IRealSenseInterface::Execute_TwoFingersPinchOpenGestureDetected(collector->interfaceDelegate, collector->tempHand);
			}

			//v_sign
			if (handData->IsGestureFired(L"v_sign", gestureData))
			{
				IRealSenseInterface::Execute_VSignGestureDetected(collector->interfaceDelegate, collector->tempHand);
			}

			//full_pinch
			if (handData->IsGestureFired(L"full_pinch", gestureData))
			{
				IRealSenseInterface::Execute_FullPinchGestureDetected(collector->interfaceDelegate, collector->tempHand);
			}

			//swipe
			if (handData->IsGestureFired(L"swipe", gestureData))
			{
				IRealSenseInterface::Execute_SwipeGestureDetected(collector->interfaceDelegate, collector->tempHand);
			}

			//wave
			if (handData->IsGestureFired(L"wave", gestureData))
			{
				IRealSenseInterface::Execute_WaveGestureDetected(collector->interfaceDelegate, collector->tempHand);
			}

		}
	}

	//Grab Face data

	//Resume next frame processing
	pp->ReleaseFrame();
}