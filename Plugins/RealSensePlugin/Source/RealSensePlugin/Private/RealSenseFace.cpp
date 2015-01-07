#include "RealSensePluginPrivatePCH.h"
#include "RealSenseFace.h"
#include "pxcsensemanager.h"
#include "pxcfacedata.h"

URealSenseFace::URealSenseFace(const FObjectInitializer &init) : UObject(init)
{
}

/*void URealSenseFace::setBone(const Leap::Bone &bone)
{
}*/

void URealSenseFace::setFromRealSenseFace(void* facePointer)
{
	//PXCFaceModule::PXCFaceData* face = (PXCHandData::PXCFaceData*)facePointer;
}