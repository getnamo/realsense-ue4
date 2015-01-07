#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseJoint.generated.h"

UCLASS(BlueprintType)
class URealSenseJoint : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Joint")
	int32 Confidence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Joint")
	FVector PositionWorld;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Joint")
	FVector PositionImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Joint")
	FRotator LocalRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Joint")
	FRotator GlobalOrientation;


	void setFromRealSenseJoint(void* jointPointer);

private:
	//class PrivateBone* _private;
};