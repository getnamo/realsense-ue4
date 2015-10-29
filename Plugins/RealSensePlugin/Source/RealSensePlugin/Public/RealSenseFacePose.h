#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseFacePose.generated.h"

UCLASS(BlueprintType)
class URealSenseFacePose : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	//pose Euler angles
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Pose")
	FRotator Rotation;

	//pose rotation as a quaternion
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Pose")
	FQuat Quaternion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Pose")
	FVector HeadPosition;

	UFUNCTION(BlueprintPure, Category = "RealSense Face Pose")
	FMatrix RotationMatrix();

	//position(angle) confidence
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Pose")
	int32 Confidence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Pose")
	int32 HeadConfidence;

	void setFromRealSenseFacePose(void* facePosePointer);

private:
	void* facePosePtr;
};