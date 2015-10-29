#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseFacePulse.generated.h"



UCLASS(BlueprintType)
class URealSenseFacePulse : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Pulse Property")
	float HeartRate;

	void setFromRealSenseFacePulse(void* facePulsePointer);

private:
};