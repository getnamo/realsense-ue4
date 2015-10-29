#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseFaceLandmarkPoint.generated.h"

//not currently utilized, to be extended

UENUM(BlueprintType)
enum RSLandmarkType
{
	LANDMARK_NOT_NAMED = 0,

	LANDMARK_EYE_RIGHT_CENTER,
	LANDMARK_EYE_LEFT_CENTER,

	LANDMARK_EYELID_RIGHT_TOP,
	LANDMARK_EYELID_RIGHT_BOTTOM,
	LANDMARK_EYELID_RIGHT_RIGHT,
	LANDMARK_EYELID_RIGHT_LEFT,

	LANDMARK_EYELID_LEFT_TOP,
	LANDMARK_EYELID_LEFT_BOTTOM,
	LANDMARK_EYELID_LEFT_RIGHT,
	LANDMARK_EYELID_LEFT_LEFT,

	LANDMARK_EYEBROW_RIGHT_CENTER,
	LANDMARK_EYEBROW_RIGHT_RIGHT,
	LANDMARK_EYEBROW_RIGHT_LEFT,

	LANDMARK_EYEBROW_LEFT_CENTER,
	LANDMARK_EYEBROW_LEFT_RIGHT,
	LANDMARK_EYEBROW_LEFT_LEFT,

	LANDMARK_NOSE_TIP,
	LANDMARK_NOSE_TOP,
	LANDMARK_NOSE_BOTTOM,
	LANDMARK_NOSE_RIGHT,
	LANDMARK_NOSE_LEFT,

	LANDMARK_LIP_RIGHT,
	LANDMARK_LIP_LEFT,

	LANDMARK_UPPER_LIP_CENTER,
	LANDMARK_UPPER_LIP_RIGHT,
	LANDMARK_UPPER_LIP_LEFT,

	LANDMARK_LOWER_LIP_CENTER,
	LANDMARK_LOWER_LIP_RIGHT,
	LANDMARK_LOWER_LIP_LEFT,

	LANDMARK_FACE_BORDER_TOP_RIGHT,
	LANDMARK_FACE_BORDER_TOP_LEFT,

	LANDMARK_CHIN
};

UENUM(BlueprintType)
enum RSLandmarksGroupType
{
	LANDMARK_GROUP_LEFT_EYE = 0x0001,
	LANDMARK_GROUP_RIGHT_EYE = 0x0002,
	LANDMARK_GROUP_RIGHT_EYEBROW = 0x0004,
	LANDMARK_GROUP_LEFT_EYEBROW = 0x0008,
	LANDMARK_GROUP_NOSE = 0x00010,
	LANDMARK_GROUP_MOUTH = 0x0020,
	LANDMARK_GROUP_JAW = 0x0040
};

UCLASS(BlueprintType)
class URealSenseFaceLandmarkPoint : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Landmark Point Source")
	int32 SourceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Landmark Point Source")
	TEnumAsByte<RSLandmarkType> SourceAlias;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Landmark Point")
	int32 ConfidenceImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Landmark Point")
	int32 ConfidenceWorld;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Landmark Point")
	FVector WorldPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Landmark Point")
	FVector2D ImagePosition;

	void setFromRealSenseLandmark(void* faceLandmark);

private:
};