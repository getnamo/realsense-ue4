#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseFace.generated.h"

//not currently utilized, to be extended

UCLASS(BlueprintType)
class URealSenseFace : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face")
	FVector WorldPosition;

	void setFromRealSenseFace(void* facePointer);

private:
};