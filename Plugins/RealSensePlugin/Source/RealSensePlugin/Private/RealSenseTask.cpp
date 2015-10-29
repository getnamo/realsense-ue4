// Fill out your copyright notice in the Description page of Project Settings.
#include "RealSensePluginPrivatePCH.h"
#include "RealSenseTask.h"
#include "RealSenseInterface.h"
#include "RealSenseJoint.h"
#include "RealSenseHand.h"
#include "RealSenseUtility.h"

#include "pxcsensemanager.h"
#include "pxchandconfiguration.h"

class DataCollector : public PXCSenseManager::Handler{
public:
	DataCollector(URealSenseTask* pointer)
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

	pxcStatus PXCAPI OnModuleProcessedFrame(pxcUID mid, PXCBase* module, PXCCapture::Sample* sample) {
		
		//UE_LOG(RealSensePluginLog, Log, TEXT("Received Frame!"));

		if (mid == PXCHandModule::CUID){
			//handData->Update();

			//Tick each component, separate this into more modules if needed
			//taskPointer->HandProducerTick();
			//taskPointer->GestureProducerTick();

			//faceData->Update();
			//taskPointer->FaceProducerTick();
		}
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

	PXCHandModule* handModule = nullptr;
	PXCHandData* handData = nullptr;
	//PXCHandData::ExtremityData extremitiesPointsNodes[2][PXCHandData::NUMBER_OF_EXTREMITIES];

	PXCFaceModule* faceModule = nullptr;
	PXCFaceData* faceData = nullptr;

	PXCSession* session;
	PXCSession::ImplVersion ver;

	//Lock-free pointer list
	TLockFreePointerList<PXCHandData::JointData> jointList;

	//Store Gamethread local pointers
	FThreadSafeBool handTickDataRead = false;

	PXCHandData::IHand* tempHandData[2];
	bool handDataRead[2];
	int numberOfDetectedHands = 0;
	
	//Hand - Joints
	PXCHandData::JointData tempJointData[2][PXCHandData::NUMBER_OF_JOINTS];
	bool jointDataReady[2][PXCHandData::NUMBER_OF_JOINTS];
	bool jointDataRead[2][PXCHandData::NUMBER_OF_JOINTS];

	//Face
	PXCFaceData::Face* facePtr = nullptr;
	int32 faces = 0;
	FThreadSafeBool faceTickDataRead = false;

	//Landmarks
	FThreadSafeBool landmarkTickDataRead = false;
	PXCFaceData::LandmarkPoint *tempLandmarkPoints = nullptr;
	int detectedPoints = 0;
	

	PXCHandConfiguration* handConfig = NULL;

	UObject* interfaceDelegate = NULL;
	URealSenseTask* taskPointer = NULL;
};

//Called on Game Thread to handle things

//Starts up thread and handles init, if everything went ok we're good to go!
void URealSenseTask::StartTask()
{
	//Make a new collector
	if (collector == nullptr)
		collector = new DataCollector(this);

	if (Runnable != nullptr)
	{
		UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::OnTaskInit: task already started, ignoring"));
		return;
	}

	UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::OnTaskInit"));

	//Setting/changing is done on the game thread, we just need one copy of this data
	tempJoint = NewObject < URealSenseJoint >(this);
	tempHand = NewObject < URealSenseHand >(this);
	tempFace = NewObject< URealSenseFace >(this);

	//All thread handling is done by parent class, just override OnTaskInit/etc
	UBackgroundTask::StartTask();

	UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::StartTask"));
}

//Stops thread and does graceful shutdown of the input module
void URealSenseTask::EndTask()
{
	//All thread handling is done by parent class, just override OnTaskShutdown/etc
	UBackgroundTask::EndTask();


	UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::EndTask"));
}


//Called on Input Thread - Allocate and delete input thread memory here
void URealSenseTask::OnTaskInit()
{
	if (!collector->IsValid()){
		UE_LOG(RealSensePluginLog, Error, TEXT("RealSense plugin disabled."));
		return;
	}

	EnableHandDetection(true);
	EnableFaceDetection(true);
	//EnableGestureDetection(true, true, FString());

	UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::OnTaskInit"));
	//UBackgroundTask::TaskInit();


}
void URealSenseTask::OnTaskShutdown()
{
	if (collector != nullptr){
		if (collector->psm != nullptr)
			collector->psm->Release();
	}

	UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::OnTaskShutdown"));
}

void URealSenseTask::BeginDestroy()
{	
	//Delete collector
	delete collector;
	collector = nullptr;
	UBackgroundTask::BeginDestroy();
}

void URealSenseTask::OnTaskTick()
{
	UBackgroundTask::OnTaskTick();

	if (collector == NULL || !collector->IsValid()){
		return;
	}
	PXCSenseManager* pp = collector->psm;

	if (consumerHasRead){
		//Resume next frame processing
		pp->ReleaseFrame();
	}
	else{
		FPlatformProcess::Sleep(0.001f);
		return;
	}	

	//Grab Frame
	pxcStatus sts = pp->AcquireFrame(true);	//,1 for a 1ms timeout. This should all be happening off-frame anyway
	consumerHasRead = false;

	collector->handData->Update();
	HandProducerTick();
	
	collector->faceData->Update();
	FaceProducerTick();

	//Check Frame status
	if (sts < PXC_STATUS_NO_ERROR)
		return;

	pp->ReleaseFrame();

	//sleep after...
	FPlatformProcess::Sleep(0.001f);	//1ms sleep..
}


void URealSenseTask::EnableHandDetection(bool enable)
{
	if (!collector->IsValid()){
		UE_LOG(RealSensePluginLog, Warning, TEXT("RealSense unavailable."));
		return;
	}

	//local pointer
	PXCSenseManager* pp = collector->psm;

	pxcStatus status = pxcStatus::PXC_STATUS_NO_ERROR;
	/* Set Module */
	if (!enable){
		collector->handModule->Release();
		// Hand Module Configuration
		PXCHandConfiguration* config = collector->handModule->CreateActiveConfiguration();
		config->DisableAllAlerts();

		config->ApplyChanges();
		config->Update();

		collector->handConfig = config;

		HandsEnabled = false;
		return;
	}
	if (enable)
	{
		status = pp->EnableHand();
		collector->handModule = pp->QueryHand();
	}

	PXCHandModule* handModule = collector->handModule;

	if (handModule == NULL || status != pxcStatus::PXC_STATUS_NO_ERROR)
	{
		UE_LOG(RealSensePluginLog, Log, TEXT("Failed to pair the hand module with I/O"));
		return;
	}

	if (pp->Init(collector) >= PXC_STATUS_NO_ERROR)
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

	HandsEnabled = true;
}

void URealSenseTask::EnableGestureDetection(bool enableAll, bool enable, FString singleGesture)
{
	if (!collector->IsValid()){
		UE_LOG(RealSensePluginLog, Warning, TEXT("RealSense unavailable."));
		return;
	}

	PXCHandConfiguration* config = collector->handConfig;

	//Null check, hand has to be enabled before we can enable gestures
	if (config == NULL)
	return;

	//enable gestures
	config->LoadGesturePack(L"navigation");

	//single gesture enabling example
	if (!singleGesture.IsEmpty()){
		if (enable)
		{
			config->EnableGesture(*singleGesture);
			GesturesEnabled = true;
		}
		else
		{
			config->DisableGesture(*singleGesture);
		}
	}
	else{
		//enable all gestures
		if (enableAll)
		{
			config->EnableAllGestures();
			UE_LOG(RealSensePluginLog, Log, TEXT("Gestures enabled"));
		}
			
		else
		{
			config->DisableAllGestures();
			UE_LOG(RealSensePluginLog, Log, TEXT("Gestures disabled"));
		}

		GesturesEnabled = enableAll;
	}

	config->ApplyChanges();
	config->Update();	
}

void URealSenseTask::EnableFaceDetection(bool enable)
{
	PXCSenseManager* pp = collector->psm;

	pxcStatus status = pp->EnableFace();

	//pp->EnableStreams()
	//pp->StreamFrames()

	collector->faceModule = pp->QueryFace();

	PXCFaceModule* faceModule = collector->faceModule;

	if (faceModule == nullptr || status != pxcStatus::PXC_STATUS_NO_ERROR)
	{
		UE_LOG(RealSensePluginLog, Log, TEXT("Failed to pair the face module with I/O"));
		return;
	}

	if (pp->IsConnected() == 0){
		if (pp->Init(collector) >= PXC_STATUS_NO_ERROR)
		{
			collector->faceData = collector->faceModule->CreateOutput();
		}
	}
	else{
		collector->faceData = collector->faceModule->CreateOutput();
	}

	FaceEnabled = enable;
}

void URealSenseTask::DisableEverything()
{
	EnableFaceDetection(false);
	EnableGestureDetection(false,false,FString());
	EnableHandDetection(false);
}

void URealSenseTask::SetInterfaceDelegate(UObject* object)
{
	UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::SetInterfaceDelegate: %d"), object);
	if (collector != nullptr)
		collector->interfaceDelegate = object;
	else
		UE_LOG(RealSensePluginLog, Warning, TEXT("URealSenseTask::SetInterfaceDelegate, collector is null!"), object);
}

void URealSenseTask::ConsumerTick(float DeltaTime)
{
	//Force a game thread tick so that old data also emits and we dont get flickering.
	//HandProducerTick();

	//Run through our data and emit if available

	//Check hand emit data
	if (HandsEnabled){
		for (pxcI32 i = 0; i < collector->numberOfDetectedHands; i++)
		{
			//Emit hand events
			tempHand->setFromRealSenseHand(collector->tempHandData[i]);
			if (collector->interfaceDelegate != nullptr)
				IRealSenseInterface::Execute_HandMoved(collector->interfaceDelegate, tempHand);

			//Emit joint events
			for (int j = 0; j < PXCHandData::NUMBER_OF_JOINTS; j++)
			{
				tempJoint->setFromRealSenseJoint(&collector->tempJointData[i][j]);
				if (collector->interfaceDelegate != nullptr)
					IRealSenseInterface::Execute_JointMoved(collector->interfaceDelegate, tempJoint);
			}
		}

		collector->handTickDataRead = true;
	}

	if (FaceEnabled)
	{

		UE_LOG(RealSensePluginLog, Log, TEXT("Face Enabled: %d"), collector->faces);
		
		if (collector->faces != 0){

			// Retrieve the face landmark data instance
			PXCFaceData::Face *face = collector->facePtr;
			tempFace->setFromRealSenseFace(face);

			if (collector->interfaceDelegate != nullptr)
				IRealSenseInterface::Execute_FaceDetected(collector->interfaceDelegate, tempFace);

			UE_LOG(RealSensePluginLog, Log, TEXT("Face Enabled Done..."));

		}
		//tempFace->QueryExpressions()

		//PXCFaceData::LandmarksData *ldata = face->QueryLandmarks();

		/*UE_LOG(RealSensePluginLog, Log, TEXT("Face Enabled: 1..."));
		PXCFaceData* faceData = collector->faceData;
		pxcI32 nfaces = faceData->QueryNumberOfDetectedFaces();

		UE_LOG(RealSensePluginLog, Log, TEXT("Face Enabled, numOfFaces: %d"), nfaces);
		for (pxcI32 i = 0; i < nfaces; i++) {
			UE_LOG(RealSensePluginLog, Log, TEXT("Face Enabled: 2..."));
			// Retrieve the face landmark data instance
			PXCFaceData::Face *face = faceData->QueryFaceByIndex(i);
			tempFace->setFromRealSenseFace(face);

			if (collector->interfaceDelegate != nullptr)
				IRealSenseInterface::Execute_FaceDetected(collector->interfaceDelegate, tempFace);

			UE_LOG(RealSensePluginLog, Log, TEXT("Face Enabled: 3..."));
			//tempFace->QueryExpressions()

			//PXCFaceData::LandmarksData *ldata = face->QueryLandmarks();
		}*/

		//collector->faceData->

		/*for (int i = 0; i < collector->detectedPoints; i++){
			PXCFaceData::LandmarkPoint* point = &collector->tempLandmarkPoints[i];


		}*/

		collector->faceTickDataRead = true;
	}

	consumerHasRead = true;
}

void runGestureOnGameThread(int a, int b, TFunction< void()> InFunction)
{
	if (a == b)
	{
		FFunctionGraphTask::CreateAndDispatchWhenReady(InFunction, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void URealSenseTask::GestureProducerTick()
{
	PXCHandData* handData = collector->handData;
	PXCHandData::GestureData gestureData;

	for (pxcI32 i = 0; i < handData->QueryNumberOfHands(); i++)
	{
		//get hand information
		PXCHandData::IHand* hand;
		
		if (handData->QueryHandData(PXCHandData::AccessOrderType::ACCESS_ORDER_BY_TIME, i, hand) == PXC_STATUS_NO_ERROR)
		{
			//spread fingers
			if (!GesturesEnabled)
				return;

				//FScopeLock lock(&GestureCriticalSection);
				//tempHand->setFromRealSenseHand(hand);
				//URealSenseHand* copyHand = DuplicateObject(tempHand, nullptr);

			if (handData->IsGestureFired(L"spreadfingers", gestureData))
			{
				runGestureOnGameThread(gestureData.handId, hand->QueryUniqueId(), [&]()
				{
					tempHand->setFromRealSenseHand(hand);
					IRealSenseInterface::Execute_SpreadFingersGestureDetected(collector->interfaceDelegate, tempHand);
				});
			}

			//fist
			if (handData->IsGestureFired(L"fist", gestureData))
			{
				runGestureOnGameThread(gestureData.handId, hand->QueryUniqueId(), [&]()
				{
					tempHand->setFromRealSenseHand(hand);
					IRealSenseInterface::Execute_FistGestureDetected(collector->interfaceDelegate, tempHand);
				});
			}

			//tap
			if (handData->IsGestureFired(L"tap", gestureData))
			{
				runGestureOnGameThread(gestureData.handId, hand->QueryUniqueId(), [&]()
				{
					tempHand->setFromRealSenseHand(hand);
					IRealSenseInterface::Execute_TapGestureDetected(collector->interfaceDelegate, tempHand);
				});
			}

			//thumb_down
			if (handData->IsGestureFired(L"thumb_down", gestureData))
			{
				runGestureOnGameThread(gestureData.handId, hand->QueryUniqueId(), [&]()
				{
					tempHand->setFromRealSenseHand(hand);
					IRealSenseInterface::Execute_ThumbDownGestureDetected(collector->interfaceDelegate, tempHand);
				});
			}

			//thumb_up

			if (handData->IsGestureFired(L"thumb_up", gestureData))
			{
				//If the gesture was fired by the same hand
				runGestureOnGameThread(gestureData.handId, hand->QueryUniqueId(), [&]()
				{
					tempHand->setFromRealSenseHand(hand);
					IRealSenseInterface::Execute_ThumbUpGestureDetected(collector->interfaceDelegate, tempHand);
				});
			}


			//two_fingers_pinch_open
			if (handData->IsGestureFired(L"two_fingers_pinch_open", gestureData))
			{
				runGestureOnGameThread(gestureData.handId, hand->QueryUniqueId(), [&]()
				{
					tempHand->setFromRealSenseHand(hand);
					IRealSenseInterface::Execute_TwoFingersPinchOpenGestureDetected(collector->interfaceDelegate, tempHand);
				});
			}

			//v_sign
			if (handData->IsGestureFired(L"v_sign", gestureData))
			{
				runGestureOnGameThread(gestureData.handId, hand->QueryUniqueId(), [&]()
				{
					tempHand->setFromRealSenseHand(hand);
					IRealSenseInterface::Execute_VSignGestureDetected(collector->interfaceDelegate, tempHand);
				});
			}


			//full_pinch
			if (handData->IsGestureFired(L"full_pinch", gestureData))
			{
				runGestureOnGameThread(gestureData.handId, hand->QueryUniqueId(), [&]()
				{
					tempHand->setFromRealSenseHand(hand);
					IRealSenseInterface::Execute_FullPinchGestureDetected(collector->interfaceDelegate, tempHand);
				});
			}

			//swipe
			if (handData->IsGestureFired(L"swipe", gestureData))
			{
				runGestureOnGameThread(gestureData.handId, hand->QueryUniqueId(), [&]()
				{
					tempHand->setFromRealSenseHand(hand);
					IRealSenseInterface::Execute_SwipeGestureDetected(collector->interfaceDelegate, tempHand);
				});
			}

			//wave
			if (handData->IsGestureFired(L"wave", gestureData))
			{
				runGestureOnGameThread(gestureData.handId, hand->QueryUniqueId(), [&]()
				{
					tempHand->setFromRealSenseHand(hand);
					IRealSenseInterface::Execute_WaveGestureDetected(collector->interfaceDelegate, tempHand);
				});
			}
		}
	}
}

void URealSenseTask::HandProducerTickOld()
{
	if (!HandsEnabled)
		return;

	//Data is up to date from earlier
	if (collector->handData == nullptr)
		return;

	PXCHandData* handData = collector->handData;

	for (pxcI32 i = 0; i < handData->QueryNumberOfHands(); i++)
	{
		//get hand information
		PXCHandData::IHand* hand;

		if (handData->QueryHandData(PXCHandData::AccessOrderType::ACCESS_ORDER_BY_TIME, i, hand) == PXC_STATUS_NO_ERROR)
		{

			{
				FScopeLock lock(&JointCriticalSection);
				collector->tempHandData[i] = hand;
			}

			//Hand data
			RunLambdaOnGameThread([&, i]()
			{
				{
					FScopeLock lock(&JointCriticalSection);

					if (!collector->handDataRead[i])
					{
						tempHand->setFromRealSenseHand(hand);//tempHandData[i]);
						if (collector->interfaceDelegate != nullptr)
							IRealSenseInterface::Execute_HandMoved(collector->interfaceDelegate, tempHand);

						//clear it
						collector->handDataRead[i] = true;
					}
				}
			});

			//Iterate Joints
			PXCHandData::JointData jointData;
			for (int j = 0; j < PXCHandData::NUMBER_OF_JOINTS; j++)
			{
				hand->QueryTrackedJoint((PXCHandData::JointType)j, jointData);

				//Copy data only if it hasn't been read
				{
					FScopeLock lock(&JointCriticalSection);
					if (collector->jointDataRead[i][j])
					{
						collector->tempJointData[i][j] = jointData;
						FVector vect = FVector(jointData.positionWorld.x, jointData.positionWorld.y, jointData.positionWorld.z);
						//UE_LOG(RealSensePluginLog, Log, TEXT("Copied Joint: %s"), *vect.ToString());

						collector->jointDataRead[i][j] = false;
					}

				}

				//This event should also emit if a frame is missed by caching the latest input
				RunLambdaOnGameThread([&, i, j]()
				{
					//{
						//FScopeLock lock(&JointCriticalSection);

						if (!collector->jointDataRead[i][j])
						{
							//FVector vect = FVector(tempJointData[i][j].positionWorld.x, tempJointData[i][j].positionWorld.y, tempJointData[i][j].positionWorld.z);
							//UE_LOG(RealSensePluginLog, Log, TEXT("Reading Joint: %s"), *vect.ToString());


							tempJoint->setFromRealSenseJoint(&collector->tempJointData[i][j]);
							if (collector->interfaceDelegate != nullptr)
								IRealSenseInterface::Execute_JointMoved(collector->interfaceDelegate, tempJoint);

							//clear it
							collector->jointDataRead[i][j] = true;
						}
					//}
				});
			}
		}

	}
}



//Newer version, without the failing
void URealSenseTask::HandProducerTick()
{
	if (!HandsEnabled)
		return;

	//Data is up to date from earlier
	if (collector->handData == nullptr)
		return;
	
	if (!collector->handTickDataRead)
		return;

	collector->handTickDataRead = false;

	PXCHandData* handData = collector->handData;
	
	collector->numberOfDetectedHands = handData->QueryNumberOfHands();

	for (pxcI32 i = 0; i < collector->numberOfDetectedHands; i++)
	{
		//get hand information
		PXCHandData::IHand* hand;

		if (handData->QueryHandData(PXCHandData::AccessOrderType::ACCESS_ORDER_BY_TIME, i, hand) == PXC_STATUS_NO_ERROR)
		{
			//Store hand data
			collector->tempHandData[i] = hand;

			//Iterate Joints - lambda capture works...
			PXCHandData::JointData jointData;
			for (int j = 0; j < PXCHandData::NUMBER_OF_JOINTS; j++)
			{
				hand->QueryTrackedJoint((PXCHandData::JointType)j, jointData);

				collector->tempJointData[i][j] = jointData;
				collector->jointDataReady[i][j] = true;

				//This event should also emit if a frame is missed by caching the latest input
				/*RunLambdaOnGameThread([&, jointData]()
				{
					PXCHandData::JointData tempJointData = jointData;

					tempJoint->setFromRealSenseJoint(&tempJointData);
					if (collector->interfaceDelegate != nullptr)
						IRealSenseInterface::Execute_JointMoved(collector->interfaceDelegate, tempJoint);
					//}
				});*/
			}
		}
	}
}

void URealSenseTask::FaceProducerTick()
{
	//Continue here, get land marks working
	//And make hands appear as well if possible

	if (!collector->faceTickDataRead)
		return;

	PXCFaceModule *faceModule = collector->psm->QueryFace();

	if (faceModule == nullptr)
		return;

	PXCFaceData* faceData = faceModule->CreateOutput();
	pxcI32 nfaces = faceData->QueryNumberOfDetectedFaces();

	collector->faces = nfaces;

	for (pxcI32 i = 0; i < nfaces; i++) {
		// Retrieve the face landmark data instance
		PXCFaceData::Face *face = faceData->QueryFaceByIndex(i);

		collector->facePtr = face;

		/*PXCFaceData::LandmarksData *ldata = face->QueryLandmarks();

		// allocate the array big enough to hold the landmark points.
		pxcI32 npoints = ldata->QueryNumPoints();

		//PXCFaceData::LandmarkPoint *points = new PXCFaceData::LandmarkPoint[npoints];

		//If the landmark points are null or array changed size, remake it
		if (collector->tempLandmarkPoints == nullptr)
		{
			collector->tempLandmarkPoints = new PXCFaceData::LandmarkPoint[npoints];

		}
		else if (collector->tempLandmarkPoints && npoints != collector->detectedPoints)
		{
			//Delete the old one remake size
			delete[] collector->tempLandmarkPoints;
			collector->tempLandmarkPoints = new PXCFaceData::LandmarkPoint[npoints];
		}

		// get the landmark data and copy it
		ldata->QueryPoints(collector->tempLandmarkPoints);

		//Copy so we can emit it later

		// Clean up
		//delete[] points;*/
	}
	collector->faceTickDataRead = false;
}

/*Raw Images

void UWindowResult::EnqueueTextureUpdate(const UTexture2D* TextureToUpdate, const WindowsTextureData* data){
if (TextureToUpdate == nullptr)
return;

struct FUploadTextureContext
{
uint8* Buffer;	// Render thread assumes ownership
uint32 BufferPitch;
FTexture2DResource* DestTextureResource;
FThreadSafeBool* readyBool;
} TextureContext =
{
(uint8*)data->pBits,
(data->windowSize.X) * 4,
(FTexture2DResource*)TextureToUpdate->Resource,
&textureReady,
};

ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
UpdateFrameTexture,
FUploadTextureContext, Context, TextureContext,
{
//Ensure our texture is valid
if (Context.DestTextureResource == nullptr)
return;

const FUpdateTextureRegion2D UpdateRegion(
0, 0,		// Dest X, Y
0, 0,		// Source X, Y
Context.DestTextureResource->GetSizeX(),	// Width
Context.DestTextureResource->GetSizeY());	// Height

//Debug timing - seems like 1ms for 1080p
//NexusUtilityTimer timer; timer.tick();

RHIUpdateTexture2D(
Context.DestTextureResource->GetTexture2DRHI(),	// Destination GPU texture
0,												// Mip map index
UpdateRegion,									// Update region
Context.BufferPitch,						// Source buffer pitch
Context.Buffer);							// Source buffer pointer

// Delete the buffer now that we've uploaded the bytes
//delete[] Context.Buffer;

//UE_LOG(LogClass, Log, TEXT("finished copy in %dms"), timer.tock());
*(Context.readyBool) = true;
}
);
return;
}

UBackgroundTask::RunLambdaOnGameThread([this, data, nextPtr](){
UTexture2D* nextPtrG = CreateTextureIfMissing(nextPtr, &data);
bgTexture = nextPtrG;

//Bail early if our texture got nuked
if (nextPtrG == nullptr){
bgTexture = nextPtrG;
return;
}

EnqueueTextureUpdate(nextPtrG, &data);

if (WindowCaptured.IsBound()){
WindowCaptured.Broadcast(this, nextPtrG);
}

textureReady = true;
});
*/
