#include "RealSensePluginPrivatePCH.h"
#include "RealSenseFaceRecognition.h"
#include "pxcsensemanager.h"
#include "pxcfacedata.h"
#include "RealSenseUtility.h"

URealSenseFaceRecognition::URealSenseFaceRecognition(const FObjectInitializer &init) : UObject(init)
{
}


int32 URealSenseFaceRecognition::RegisterUser()
{
	PXCFaceData::RecognitionData* recognition = (PXCFaceData::RecognitionData*)recognitionPtr;
	return recognition->RegisterUser();
}


void URealSenseFaceRecognition::UnregisterUser()
{
	PXCFaceData::RecognitionData* recognition = (PXCFaceData::RecognitionData*)recognitionPtr;
	recognition->UnregisterUser();
}

void URealSenseFaceRecognition::setFromRealSenseFaceRecognition(void* faceRecognitionPointer)
{
	recognitionPtr = faceRecognitionPointer;
	PXCFaceData::RecognitionData* recognition = (PXCFaceData::RecognitionData*)recognitionPtr;

	IsRegistered = 1 == recognition->IsRegistered();

	UserId = recognition->QueryUserID();
}