// Fill out your copyright notice in the Description page of Project Settings.
#include "RealSensePluginPrivatePCH.h"
#include "RealSenseHandWorker.h"
#include "RealSenseInterface.h"
#include "RealSenseJoint.h"
#include "RealSenseHand.h"
#include "RealSenseFaceLandmarks.h"
#include "RealSenseFaceLandmarkPoint.h"
#include "RealSenseUtility.h"

#include "pxcsensemanager.h"
#include "pxchandconfiguration.h"

class DataCollector : public PXCSenseManager::Handler{
public:
	DataCollector(URealSenseHandWorker* pointer)
	{
		UE_LOG(RealSensePluginLog, Log, TEXT("DataCollector::DataCollector"));

		//set our task pointer
		taskPointer = pointer;

		// create the PXCSenseManager
		psm = PXCSenseManager::CreateInstance();
		if (!psm)
		{
			UE_LOG(RealSensePluginLog, Error, TEXT("RealSense failed to create instance, Unable to create the PXCSenseManager, do you have the correct runtime installed?."));
			return;
		}
		// Retrieve the underlying session created by the PXCSenseManager.
		// The returned instance is an PXCSenseManager internally managed object.
		session = psm->QuerySession();
		if (session == NULL)
		{
			UE_LOG(RealSensePluginLog, Error, TEXT("RealSense failed to create instance, Session not created by PXCSenseManager"));
			return;
		}

		//Set the version, log it for debugging.
		ver = session->QueryVersion();
		UE_LOG(RealSensePluginLog, Log, TEXT("Plugin v%s, Intel RSSDK Version %d.%d"), TEXT(PLUGIN_VERSION), ver.major, ver.minor);

		//log all available modules that are automatically loaded with the RSSDK
		for (int i = 0;; i++) {
			PXCSession::ImplDesc desc;
			if (session->QueryImpl(0, i, &desc) < PXC_STATUS_NO_ERROR) break; 
			// Print the module friendly name and iuid (interface unique ID)
			UE_LOG(RealSensePluginLog, Log, TEXT("Module[%d]: %s\n"), i, desc.friendlyName);
			UE_LOG(RealSensePluginLog, Log, TEXT(" iuid = %x\n"), desc.iuid);
		}
	}
	~DataCollector()
	{
		psm->Release();
		psm = nullptr;
	}

	//Handler events fired by psm
	pxcStatus PXCAPI OnConnect(PXCCapture::Device* device, pxcBool connected) {
		UE_LOG(RealSensePluginLog, Log, TEXT("Device Connected: %d"), connected);
		return PXC_STATUS_NO_ERROR;
	}

	pxcStatus PXCAPI OnModuleSetProfile(pxcUID mid, PXCBase* module) {
		UE_LOG(RealSensePluginLog, Log, TEXT("OnModuleSetProfile: %d"), mid);
		return PXC_STATUS_NO_ERROR;
	}

	pxcStatus PXCAPI OnNewSample(pxcUID mid, PXCCapture::Sample* sample) {
		UE_LOG(RealSensePluginLog, Log, TEXT("OnNewSample: %d"), mid);
		return PXC_STATUS_NO_ERROR;
	}

	void PXCAPI OnStatus(pxcUID mid, pxcStatus sts) {
		UE_LOG(RealSensePluginLog, Log, TEXT("OnStatus: %d"), mid);
	}

	bool IsValid(){
		return psm != nullptr;
	}

	PXCSenseManager* psm = nullptr;
	PXCHandConfiguration* handConfig = nullptr;

	PXCHandModule* handModule = nullptr;
	PXCHandData* handData = nullptr;

	PXCHandData::JointData allJoints[2][22];
	int numHands = 0;

	PXCFaceModule* faceModule = nullptr;
	PXCFaceData* faceData = nullptr;

	PXCFaceData::LandmarkPoint allPoints[2][78];	//potentially 2 faces...
	int numFaces = 0;

	PXCSession* session;
	PXCSession::ImplVersion ver;

	UObject* interfaceDelegate = NULL;
	URealSenseHandWorker* taskPointer = NULL;
};

void URealSenseHandWorker::StartTask()
{
	collector = new DataCollector(this);
	EnableHandDetection(true);
	//EnableGestureDetection(true, FString("v_sign"));

	collector->psm->Init();


	if (Runnable != nullptr)
	{
		UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::OnTaskInit: task already started, ignoring"));
		return;
	}

	UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::OnTaskInit"));

	//Setting/changing is done on the game thread, we just need one copy of this data
	tempJoint = NewObject < URealSenseJoint >(this);
	tempHand = NewObject < URealSenseHand >(this);

	UBackgroundTask::StartTask();
}

void URealSenseHandWorker::EndTask()
{
	HandEnabled = false;
	delete collector;
}

void URealSenseHandWorker::ConsumerTick(float DeltaTime)
{
	//if (consumerHasRead)
	//	return;

	//Optional with this architecture

	//hands
	for (int i = 0; i < collector->numHands; i++)
	{
		for (int j = 0; j < 22; j++)
		{
			tempJoint->setFromRealSenseJoint(&collector->allJoints[i][j]);
			if (collector->interfaceDelegate != nullptr)
				IRealSenseInterface::Execute_JointMoved(collector->interfaceDelegate, tempJoint);
		}
	}

	consumerHasRead = true;
}

void URealSenseHandWorker::EnableHandDetection(bool enable)
{
	// Create the SenseManager instance
	PXCSenseManager *sm = collector->psm;

	// Enable face tracking
	sm->EnableHand();

	// Get a face instance for feature configuration
	collector->handModule = sm->QueryHand();

	// Create a storage for output
	collector->handData = collector->handModule->CreateOutput();

	PXCHandConfiguration* config = collector->handModule->CreateActiveConfiguration();
	config->EnableNormalizedJoints(true);
	if (true) config->SetTrackingMode(PXCHandData::TRACKING_MODE_FULL_HAND);
	//config->EnableAllAlerts();
	//config->EnableSegmentationImage(true);

	config->ApplyChanges();
	config->Update();

	collector->handConfig = config;

	HandEnabled = true;
}

void URealSenseHandWorker::EnableGestureDetection(bool enable, FString singleGesture)
{
	// Create the SenseManager instance
	PXCSenseManager *sm = collector->psm;

	// Get an instance of PXCHandConfiguration

	PXCHandConfiguration* config = collector->handModule->CreateActiveConfiguration();
	if (enable)
		config->EnableGesture(*singleGesture);
	else
		config->DisableGesture(*singleGesture);

	config->ApplyChanges();
	config->Update();

}

void URealSenseHandWorker::SetInterfaceDelegate(UObject* object)
{
	UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::SetInterfaceDelegate: %d"), object);
	if (collector != nullptr)
		collector->interfaceDelegate = object;
	else
		UE_LOG(RealSensePluginLog, Warning, TEXT("URealSenseTask::SetInterfaceDelegate, collector is null!"), object);
}

void URealSenseHandWorker::OnTaskTick()
{
	UBackgroundTask::OnTaskTick();

	if (collector == nullptr || !collector->IsValid()){
		return;
	}
	PXCSenseManager* sm = collector->psm;

	//if (consumerHasRead && collector){
		sm->ReleaseFrame();

		//Grab Frame
		pxcStatus sts = sm->AcquireFrame(false);	//,1 for a 1ms timeout. This should all be happening off-frame anyway

		//Check Frame status
		if (sts < PXC_STATUS_NO_ERROR)
			return;

		collector->handData->Update();
		ProcessHandData();

		consumerHasRead = false;
	//}

	//sleep after...
	FPlatformProcess::Sleep(0.01f);	//10ms sleep..
}

void URealSenseHandWorker::OnTaskInit()
{
	UBackgroundTask::OnTaskInit();
}

void URealSenseHandWorker::OnTaskShutdown()
{
	UBackgroundTask::OnTaskShutdown();
}

void URealSenseHandWorker::ProcessHandData()
{
	if (!HandEnabled)
		return;
	auto handData = collector->handData;
	//handData->Update();
	int NumHands = handData->QueryNumberOfHands();

	//UE_LOG(RealSensePluginLog, Log, TEXT("ProcessHandData: %d"), NumHands);
	collector->numHands = NumHands;

	for (int i = 0; i < NumHands; i++)
	{
		PXCHandData::IHand* hand;
		if (handData->QueryHandData(PXCHandData::AccessOrderType::ACCESS_ORDER_BY_TIME, i, hand) == PXC_STATUS_NO_ERROR)
		{
			//Emit hand events
			RunLambdaOnGameThread([&, hand]{
				tempHand->setFromRealSenseHand(hand);
				if (collector->interfaceDelegate != nullptr)
					IRealSenseInterface::Execute_HandMoved(collector->interfaceDelegate, tempHand);
			});

			//Emit joint events
			for (int j = 0; j < PXCHandData::NUMBER_OF_JOINTS; j++)
			{
				PXCHandData::JointData jointData;
				hand->QueryTrackedJoint((PXCHandData::JointType)j, jointData);

				//Copy joints
				collector->allJoints[i][j] = jointData;

				/*RunLambdaOnGameThread([&, jointData]{
					auto joint = jointData;
					tempJoint->setFromRealSenseJoint(&joint);
					if (collector->interfaceDelegate != nullptr)
						IRealSenseInterface::Execute_JointMoved(collector->interfaceDelegate, tempJoint);
				});*/
			}

			//emit gestures
			CheckGesturesForHand(hand);
		}
	}
}

void runGestureOnGameThreadIfSame(int a, int b, TFunction< void()> InFunction)
{
	if (a == b)
	{
		FFunctionGraphTask::CreateAndDispatchWhenReady(InFunction, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void URealSenseHandWorker::CheckGesturesForHand(PXCHandData::IHand* hand)
{
	PXCHandData::GestureData gestureData;
	PXCHandData* handData = collector->handData;
	int uniqueId = hand->QueryUniqueId();

	if (handData->IsGestureFired(L"spreadfingers", gestureData))
	{
		runGestureOnGameThreadIfSame(gestureData.handId, uniqueId, [&, hand]()
		{
			tempHand->setFromRealSenseHand(hand);
			IRealSenseInterface::Execute_SpreadFingersGestureDetected(collector->interfaceDelegate, tempHand);
		});
	}

	//fist
	if (handData->IsGestureFired(L"fist", gestureData))
	{
		runGestureOnGameThreadIfSame(gestureData.handId, uniqueId, [&, hand]()
		{
			tempHand->setFromRealSenseHand(hand);
			IRealSenseInterface::Execute_FistGestureDetected(collector->interfaceDelegate, tempHand);
		});
	}

	//tap
	if (handData->IsGestureFired(L"tap", gestureData))
	{
		runGestureOnGameThreadIfSame(gestureData.handId, uniqueId, [&, hand]()
		{
			tempHand->setFromRealSenseHand(hand);
			IRealSenseInterface::Execute_TapGestureDetected(collector->interfaceDelegate, tempHand);
		});
	}

	//thumb_down
	if (handData->IsGestureFired(L"thumb_down", gestureData))
	{
		runGestureOnGameThreadIfSame(gestureData.handId, uniqueId, [&, hand]()
		{
			tempHand->setFromRealSenseHand(hand);
			IRealSenseInterface::Execute_ThumbDownGestureDetected(collector->interfaceDelegate, tempHand);
		});
	}

	//thumb_up

	if (handData->IsGestureFired(L"thumb_up", gestureData))
	{
		//If the gesture was fired by the same hand
		runGestureOnGameThreadIfSame(gestureData.handId, uniqueId, [&, hand]()
		{
			tempHand->setFromRealSenseHand(hand);
			IRealSenseInterface::Execute_ThumbUpGestureDetected(collector->interfaceDelegate, tempHand);
		});
	}


	//two_fingers_pinch_open
	if (handData->IsGestureFired(L"two_fingers_pinch_open", gestureData))
	{
		runGestureOnGameThreadIfSame(gestureData.handId, uniqueId, [&, hand]()
		{
			tempHand->setFromRealSenseHand(hand);
			IRealSenseInterface::Execute_TwoFingersPinchOpenGestureDetected(collector->interfaceDelegate, tempHand);
		});
	}

	//v_sign
	if (handData->IsGestureFired(L"v_sign", gestureData))
	{
		runGestureOnGameThreadIfSame(gestureData.handId, uniqueId, [&, hand]()
		{
			tempHand->setFromRealSenseHand(hand);
			IRealSenseInterface::Execute_VSignGestureDetected(collector->interfaceDelegate, tempHand);
		});
	}


	//full_pinch
	if (handData->IsGestureFired(L"full_pinch", gestureData))
	{
		runGestureOnGameThreadIfSame(gestureData.handId, uniqueId, [&, hand]()
		{
			tempHand->setFromRealSenseHand(hand);
			IRealSenseInterface::Execute_FullPinchGestureDetected(collector->interfaceDelegate, tempHand);
		});
	}

	//swipe
	if (handData->IsGestureFired(L"swipe", gestureData))
	{
		runGestureOnGameThreadIfSame(gestureData.handId, uniqueId, [&, hand]()
		{
			tempHand->setFromRealSenseHand(hand);
			IRealSenseInterface::Execute_SwipeGestureDetected(collector->interfaceDelegate, tempHand);
		});
	}

	//wave
	if (handData->IsGestureFired(L"wave", gestureData))
	{
		runGestureOnGameThreadIfSame(gestureData.handId, uniqueId, [&, hand]()
		{
			tempHand->setFromRealSenseHand(hand);
			IRealSenseInterface::Execute_WaveGestureDetected(collector->interfaceDelegate, tempHand);
		});
	}
}
