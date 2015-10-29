#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseFaceRecognition.generated.h"



UCLASS(BlueprintType)
class URealSenseFaceRecognition : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Pose")
	bool IsRegistered;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Pose")
	int32 UserId;


	/**
	@brief Register a user in the Recognition database.
	@return The unique user ID assigned to the registered face by the Recognition module.
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face Recognition")
	int32 RegisterUser();

	/**
	@brief Removes a user from the Recognition database.
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face Recognition")
	void UnregisterUser();

	void setFromRealSenseFaceRecognition(void* faceRecognition);

private:
	void* recognitionPtr;
};