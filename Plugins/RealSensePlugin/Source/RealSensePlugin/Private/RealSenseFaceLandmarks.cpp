#include "RealSensePluginPrivatePCH.h"
#include "RealSenseFaceLandmarks.h"
#include "pxcsensemanager.h"
#include "pxcfacedata.h"
#include "RealSenseUtility.h"


URealSenseFaceLandmarks::URealSenseFaceLandmarks(const FObjectInitializer &init) : UObject(init)
{
	//Pre-fill array on creation
	for (int i = 0; i < RS_MAX_LANDMARK_POINTS; i++)
	{
		//URealSenseFaceLandmarkPoint* point = init.CreateDefaultSubobject<URealSenseFaceLandmarkPoint>(this, TEXT("Point" + i));
		URealSenseFaceLandmarkPoint* point = NewObject<URealSenseFaceLandmarkPoint>(this, TEXT("point"));
		landmarkPointArray.Add(point);
	}
}
TArray<class URealSenseFaceLandmarkPoint*> URealSenseFaceLandmarks::QueryPoints()
{
	PXCFaceData::LandmarksData* landmarks = (PXCFaceData::LandmarksData*)landmarksPtr;

	PXCFaceData::LandmarkPoint* rsPoints = new PXCFaceData::LandmarkPoint[NumPoints];
	landmarks->QueryPoints(rsPoints);

	//make a copy result
	TArray<URealSenseFaceLandmarkPoint*> outPoints;
	
	for (int i = 0; i < NumPoints; i++)
	{
		auto point = landmarkPointArray[i];
		auto rsPoint = rsPoints[i];
		point->setFromRealSenseLandmark(&rsPoint);
		outPoints.Add(point);
	}

	delete rsPoints;

	return outPoints;
}


bool URealSenseFaceLandmarks::QueryPoint(int32 index, const class URealSenseFaceLandmarkPoint* outPoint)
{
	PXCFaceData::LandmarksData* landmarks = (PXCFaceData::LandmarksData*)landmarksPtr;
	PXCFaceData::LandmarkPoint point;
	landmarks->QueryPoint(index, &point);
	return false;
}


int32 URealSenseFaceLandmarks::QueryNumPointsByGroup(RSLandmarksGroupType groupFlags)
{
	PXCFaceData::LandmarksData* landmarks = (PXCFaceData::LandmarksData*)landmarksPtr;
	return (int32)landmarks->QueryNumPointsByGroup((PXCFaceData::LandmarksGroupType)groupFlags);
}


bool URealSenseFaceLandmarks::QueryPointsByGroup(RSLandmarksGroupType groupFlags, TArray<URealSenseFaceLandmarkPoint*> outPoints)
{
	PXCFaceData::LandmarksData* landmarks = (PXCFaceData::LandmarksData*)landmarksPtr;

	int32 NumGroupPoints = QueryNumPointsByGroup(groupFlags);
	PXCFaceData::LandmarkPoint* rsPoints = new PXCFaceData::LandmarkPoint[NumGroupPoints];
	bool success = 1 == landmarks->QueryPointsByGroup((PXCFaceData::LandmarksGroupType)groupFlags, rsPoints);

	if (!success)
		return false;
	
	for (int i = 0; i < NumGroupPoints; i++)
	{
		auto point = landmarkPointArray[i];
		auto rsPoint = rsPoints[i];
		point->setFromRealSenseLandmark(&rsPoint);
		outPoints.Add(point);
	}

	delete rsPoints;

	return true;
}

int32 URealSenseFaceLandmarks::QueryPointIndex(RSLandmarkType name)
{
	PXCFaceData::LandmarksData* landmarks = (PXCFaceData::LandmarksData*)landmarksPtr;
	return (int32)landmarks->QueryPointIndex((PXCFaceData::LandmarkType)name);
}

void URealSenseFaceLandmarks::setFromRealSenseLandmark(void* landmarkPointer)
{
	if (landmarkPointer == nullptr)
		return;
	landmarksPtr = landmarkPointer;
	PXCFaceData::LandmarksData* landmarks = (PXCFaceData::LandmarksData*)landmarksPtr;

	NumPoints = landmarks->QueryNumPoints();
}


