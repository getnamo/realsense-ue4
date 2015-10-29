#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseFaceLandmarkPoint.h"
#include "RealSenseFaceLandmarks.generated.h"

//not currently utilized, to be extended

UCLASS(BlueprintType)
class URealSenseFaceLandmarks : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Landmarks")
	int32 NumPoints;

	/*
	* Assigns all the points to TArray.
	*/
	UFUNCTION(BlueprintPure, Category = "RealSense Face Landmarks")
	TArray<class URealSenseFaceLandmarkPoint*> QueryPoints();

	/*
	* Assigns point matched to index to outPoint.
	* Returns true if data and outPoint exists and index is correct, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face Landmarks")
	bool QueryPoint(int32 index, const class URealSenseFaceLandmarkPoint* outPoint);

	/*
	* Returns the number of tracked landmarks in groupFlags.
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face Landmarks")
	int32 QueryNumPointsByGroup(RSLandmarksGroupType groupFlags);

	/*
	* Assigns points matched to groupFlags to outPoints.
	* User is expected to allocate outPoints to size bigger than the group size - point contains the original source (index + name).
	* Returns true if data and parameters exist, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face Landmarks")
	bool QueryPointsByGroup(RSLandmarksGroupType groupFlags, TArray<class URealSenseFaceLandmarkPoint*> outPoints);

	/*
	* Mapping function -> retrieves index corresponding to landmark's name.
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face Landmarks")
	int32 QueryPointIndex(RSLandmarkType name);

	void setFromRealSenseLandmark(void* faceLandmark);

private:
	void setLmPointArrayFromRSPoints(void* points);

	TArray<URealSenseFaceLandmarkPoint*> landmarkPointArray;
	void* landmarksPtr;
};