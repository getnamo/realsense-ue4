#include "RealSensePluginPrivatePCH.h"
#include "RealSenseFaceLandmarkPoint.h"
#include "pxcsensemanager.h"
#include "pxcfacedata.h"
#include "RealSenseUtility.h"

URealSenseFaceLandmarkPoint::URealSenseFaceLandmarkPoint(const FObjectInitializer &init) : UObject(init)
{
}

void URealSenseFaceLandmarkPoint::setFromRealSenseLandmark(void* landmarkPointer)
{
	PXCFaceData::LandmarkPoint* point = (PXCFaceData::LandmarkPoint*)landmarkPointer;

	SourceIndex = point->source.index;

	SourceAlias = (RSLandmarkType)point->source.alias;

	ConfidenceImage = point->confidenceImage;

	ConfidenceWorld = point->confidenceWorld;

	WorldPosition = ueVectorFromRSVector(point->world);

	ImagePosition = ueVector2DFromPXCPointF32(point->image);

}