// Fill out your copyright notice in the Description page of Project Settings.
#include "RealSensePluginPrivatePCH.h"
#include "RealSenseFaceWorker.h"
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
	DataCollector(URealSenseFaceWorker* pointer)
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
		/*for (int i = 0;; i++) {
			PXCSession::ImplDesc desc;
			if (session->QueryImpl(0, i, &desc) < PXC_STATUS_NO_ERROR) break; 
			// Print the module friendly name and iuid (interface unique ID)
			UE_LOG(RealSensePluginLog, Log, TEXT("Module[%d]: %s\n"), i, desc.friendlyName);
			UE_LOG(RealSensePluginLog, Log, TEXT(" iuid = %x\n"), desc.iuid);
		}*/
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

	PXCFaceModule* faceModule = nullptr;
	PXCFaceData* faceData = nullptr;

	PXCFaceData::LandmarkPoint allPoints[2][78];	//potentially 2 faces...
	int numFaces = 0;

	PXCSession* session;
	PXCSession::ImplVersion ver;

	UObject* interfaceDelegate = NULL;
	URealSenseFaceWorker* taskPointer = NULL;
};

void URealSenseFaceWorker::StartTask()
{
	collector = new DataCollector(this);
	EnableFaceDetection(true);

	collector->psm->Init();


	if (Runnable != nullptr)
	{
		UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::OnTaskInit: task already started, ignoring"));
		return;
	}

	UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::OnTaskInit"));

	//Setting/changing is done on the game thread, we just need one copy of this data
	tempFace = NewObject< URealSenseFace >(this);
	tempLandmark = NewObject <URealSenseFaceLandmarkPoint >(this);

	UBackgroundTask::StartTask();
}

void URealSenseFaceWorker::EndTask()
{
	FaceEnabled = false;
	delete collector;
}

void URealSenseFaceWorker::ConsumerTick(float DeltaTime)
{
	//if (consumerHasRead)
	//	return;

	//Optional with this architecture
	ProcessFaceData();

	//face
	for (int i = 0; i < collector->numFaces; i++)
	{
		for (int j = 0; j < 78; j++)
		{
			tempLandmark->setFromRealSenseLandmark(&collector->allPoints[i][j]);
			if (collector->interfaceDelegate != nullptr)
				IRealSenseInterface::Execute_FaceLandmarkMoved(collector->interfaceDelegate, tempLandmark);
		}
	}

	consumerHasRead = true;
}

void URealSenseFaceWorker::EnableFaceDetection(bool enable)
{
	// Create the SenseManager instance
	PXCSenseManager *sm = collector->psm;

	// Enable face tracking
	sm->EnableFace();

	// Get a face instance for feature configuration
	collector->faceModule = sm->QueryFace();

	collector->faceData = collector->faceModule->CreateOutput();

	FaceEnabled = true;
}

void URealSenseFaceWorker::SetInterfaceDelegate(UObject* object)
{
	UE_LOG(RealSensePluginLog, Log, TEXT("URealSenseTask::SetInterfaceDelegate: %d"), object);
	if (collector != nullptr)
		collector->interfaceDelegate = object;
	else
		UE_LOG(RealSensePluginLog, Warning, TEXT("URealSenseTask::SetInterfaceDelegate, collector is null!"), object);
}

void URealSenseFaceWorker::OnTaskTick()
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

		collector->faceData->Update();
		ProcessFaceData();

		consumerHasRead = false;
	//}

	//sleep after...
	FPlatformProcess::Sleep(0.01f);	//10ms sleep..
}

void URealSenseFaceWorker::OnTaskInit()
{
	UBackgroundTask::OnTaskInit();
}

void URealSenseFaceWorker::OnTaskShutdown()
{
	UBackgroundTask::OnTaskShutdown();
}

void URealSenseFaceWorker::ProcessFaceData()
{
	if (!FaceEnabled)
		return;
	auto faceData = collector->faceData;
	//faceData->Update();
	int NumFaces = faceData->QueryNumberOfDetectedFaces();

	//UE_LOG(RealSensePluginLog, Log, TEXT("ProcessFaceData: %d"), NumFaces);
	collector->numFaces = NumFaces;

	auto faces = faceData->QueryFaces(&NumFaces);
	for (int i = 0; i < NumFaces; i++)
	{
		auto face = faces[i];
		
		RunLambdaOnGameThread([&, face]{
			tempFace->setFromRealSenseFace(face);
			if (collector->interfaceDelegate != nullptr)
				IRealSenseInterface::Execute_FaceDetected(collector->interfaceDelegate, tempFace);
		});

		auto pointCount = face->QueryLandmarks()->QueryNumPoints();
		PXCFaceData::LandmarkPoint* landmarkPoints = new PXCFaceData::LandmarkPoint[pointCount];
		face->QueryLandmarks()->QueryPoints(landmarkPoints);

		for (int j = 0; j < pointCount; j++)
		{
			auto point = landmarkPoints[j];

			collector->allPoints[i][j] = point;

			/*RunLambdaOnGameThread([&, point]{
				auto gPoint = point;
				tempLandmark->setFromRealSenseLandmark(&gPoint);

				if (collector->interfaceDelegate != nullptr)
					IRealSenseInterface::Execute_FaceLandmarkMoved(collector->interfaceDelegate, tempLandmark);
			});*/
		}

		delete landmarkPoints;
	}
}
