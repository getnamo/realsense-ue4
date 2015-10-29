#include "RealSensePluginPrivatePCH.h"
#include "RealSenseFaceDetection.h"
#include "pxcsensemanager.h"
#include "pxcfacedata.h"
#include "RealSenseUtility.h"

URealSenseFaceDetection::URealSenseFaceDetection(const FObjectInitializer &init) : UObject(init)
{
}

void URealSenseFaceDetection::setFromRealSenseDetection(void* faceDetectionPointer)
{
	PXCFaceData::DetectionData* detection = (PXCFaceData::DetectionData*)faceDetectionPointer;

	PXCRectI32 rect;
	detection->QueryBoundingRect(&rect);
	BoundingRect = uePRectFromRSRect(rect);

	detection->QueryFaceAverageDepth(&FaceAverageDepth);
}