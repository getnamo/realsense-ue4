#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseTypes.h"
#include "RealSenseFaceDetection.generated.h"



UCLASS(BlueprintType)
class URealSenseFaceDetection : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Detection")
	FPIntRect BoundingRect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Detection")
	float FaceAverageDepth;

	void setFromRealSenseDetection(void* faceDetectionPointer);
};