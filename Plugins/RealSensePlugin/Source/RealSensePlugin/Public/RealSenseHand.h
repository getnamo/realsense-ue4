#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseHand.generated.h"

UENUM(BlueprintType)
enum RealSenseBodySide
{
	BODY_SIDE_UNKNOWN,
	BODY_SIDE_LEFT,
	BODY_SIDE_RIGHT
};

UENUM(BlueprintType)
enum RealSenseJointType
{
	/// The center of the wrist
	JOINT_WRIST = 0
	, JOINT_CENTER /// The center of the palm
	, JOINT_THUMB_BASE /// Thumb finger joint 1 (base)
	, JOINT_THUMB_JT1 /// Thumb finger joint 2
	, JOINT_THUMB_JT2 /// Thumb finger joint 3
	, JOINT_THUMB_TIP /// Thumb finger joint 4 (fingertip)
	, JOINT_INDEX_BASE /// Index finger joint 1 (base)
	, JOINT_INDEX_JT1 /// Index finger joint 2
	, JOINT_INDEX_JT2 /// Index finger joint 3
	, JOINT_INDEX_TIP /// Index finger joint 4 (fingertip)
	, JOINT_MIDDLE_BASE /// Middle finger joint 1 (base)
	, JOINT_MIDDLE_JT1 /// Middle finger joint 2
	, JOINT_MIDDLE_JT2 /// Middle finger joint 3
	, JOINT_MIDDLE_TIP /// Middle finger joint 4 (fingertip)
	, JOINT_RING_BASE /// Ring finger joint 1 (base)
	, JOINT_RING_JT1 /// Ring finger joint 2
	, JOINT_RING_JT2 /// Ring finger joint 3
	, JOINT_RING_TIP /// Ring finger joint 4 (fingertip)
	, JOINT_PINKY_BASE /// Pinky finger joint 1 (base)
	, JOINT_PINKY_JT1 /// Pinky finger joint 2
	, JOINT_PINKY_JT2 /// Pinky finger joint 3
	, JOINT_PINKY_TIP /// Pinky finger joint 4 (fingertip)
};

UCLASS(BlueprintType)
class URealSenseHand : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Hand")
	TEnumAsByte<RealSenseBodySide> BodySide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Hand")
	FVector MassCenterWorld;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Hand")
	FRotator PalmOrientation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Hand")
	int32 UserId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Hand")
	int32 UniqueId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Hand")
	int32 Openness;

	UFUNCTION(BlueprintCallable, Category = "RealSense Hand")
	class URealSenseJoint* QueryTrackedJoint(RealSenseJointType jointLabel);

	void setFromRealSenseHand(void* handPointer);

private:
	URealSenseJoint* QueryJoint;
	void* RSHand;
};