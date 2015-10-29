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
//#include "RealSenseTask.h"
#include "RealSenseHandWorker.h"
#include "RealSenseFaceWorker.h"

IMPLEMENT_MODULE(FRealSensePlugin, RealSensePlugin)

//Init and Runtime
void FRealSensePlugin::StartupModule()
{
	handWorker = NewObject < URealSenseHandWorker >();		//NB this may fail on gc, annnnd it doesn't, nice
	handWorker->AddToRoot();
	handWorker->StartTask();

	faceWorker = NewObject < URealSenseFaceWorker >();
	faceWorker->AddToRoot();
	faceWorker->StartTask();
}

void FRealSensePlugin::ShutdownModule()
{
	if (handWorker != nullptr && handWorker->IsValidLowLevel()){
		handWorker->EndTask();
		handWorker->RemoveFromRoot();
	}

	if (faceWorker != nullptr && handWorker->IsValidLowLevel()){
		faceWorker->EndTask();
		faceWorker->RemoveFromRoot();
	}

	//delete collector;
}


void FRealSensePlugin::EnableStreams()
{
	//Just try depth for now
	//collector->psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH);
	//collector->psm->EnableStream(PXCCapture::STREAM_TYPE_IR);
	//collector->psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR);
}

//for now this enables all gestures, future version will have control over this
void FRealSensePlugin::EnableGestureDetection(bool enable, FString gesture)
{
	handWorker->EnableGestureDetection(enable, gesture);
}

void FRealSensePlugin::EnableAllGestureDetection(bool enableAll)
{
	handWorker->EnableGestureDetection(enableAll, FString());
}

void FRealSensePlugin::EnableFaceDetection(bool enable)
{
	faceWorker->EnableFaceDetection(enable);
}

void FRealSensePlugin::EnableHandDetection(bool enable)
{
	handWorker->EnableHandDetection(enable);
}

void FRealSensePlugin::SetInterfaceDelegate(UObject* object)
{
	if (object == NULL)
	{
		if (handWorker != nullptr)
		{
			handWorker->SetInterfaceDelegate(NULL);
		}
		if (faceWorker != nullptr)
		{
			faceWorker->SetInterfaceDelegate(NULL);
		}
	}
	else if (object->GetClass()->ImplementsInterface(URealSenseInterface::StaticClass()))
	{
		if (handWorker != nullptr)
		{
			handWorker->SetInterfaceDelegate(object);
		}
		if (faceWorker != nullptr)
		{
			faceWorker->SetInterfaceDelegate(object);
		}
	}
}

//Delegate methods
void FRealSensePlugin::RealSenseTick(float DeltaTime)
{
	handWorker->ConsumerTick(DeltaTime);
	faceWorker->ConsumerTick(DeltaTime);
}