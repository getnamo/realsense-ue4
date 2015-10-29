#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseFace.generated.h"

UCLASS(BlueprintType)
class URealSenseFace : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	//TODO: add gaze support

	/*
	* Returns user ID.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Property")
	int32 UserID;

	/*
	* Returns user's detection data instance - none if it is not enabled.
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face")
	class URealSenseFaceDetection* QueryDetection();

	/*
	* Returns user's landmarks data instance - none if it is not enabled.
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face")
	class URealSenseFaceLandmarks* QueryLandmarks();

	/*
	* Returns user's pose data - none if it is not enabled.
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face")
	class URealSenseFacePose* QueryPose();

	/*
	* Returns user's expressions data - none if it not enabled.
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face")
	class URealSenseFaceExpressions* QueryExpressions();

	/*
	* Returns user's recognition data - none if it is not enabled.
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face")
	class URealSenseFaceRecognition* QueryRecognition();

	/*
	* Returns user's pulse data - none if it is not enabled.
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face")
	class URealSenseFacePulse* QueryPulse();


	void setFromRealSenseFace(void* facePointer);

private:
	class URealSenseFaceDetection* detection;
	class URealSenseFaceLandmarks* landmarks;
	class URealSenseFacePose* pose;
	class URealSenseFaceExpressions* expressions;
	class URealSenseFaceRecognition* recognition;
	class URealSenseFacePulse* pulse;
	void* facePtr;
};