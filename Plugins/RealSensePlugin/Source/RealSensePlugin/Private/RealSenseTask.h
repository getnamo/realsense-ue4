// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackgroundTask.h"
#include "RealSenseUtility.h"
#include "RealSenseTask.generated.h"



class DataCollector;
class URealSenseJoint;
class URealSenseHand;
class URealSenseFace;
class URealSenseFaceExpressions;

/**
 * 
 */
UCLASS()
class URealSenseTask : public UBackgroundTask 
{
	friend class DataCollector;

	GENERATED_BODY()
public:
	//Called on Game thread
	void StartTask() override;
	void EndTask() override;
	void ConsumerTick(float DeltaTime) override;

	//Custom Game thread interaction
	void EnableHandDetection(bool enable);
	void EnableGestureDetection(bool enableAll, bool enable, FString singleGesture);
	void EnableFaceDetection(bool enable);
	void DisableEverything();

	void SetInterfaceDelegate(UObject* object);

protected:
	//Temp UObjects
	URealSenseJoint* tempJoint = nullptr;
	URealSenseHand* tempHand = nullptr;
	URealSenseFace* tempFace = nullptr;
	URealSenseFaceExpressions* tempExpressions = nullptr;

	FThreadSafeBool consumerHasRead = false;

	bool GesturesEnabled;
	void GestureProducerTick();

	bool HandsEnabled;
	void HandProducerTick();
	void HandProducerTickOld();

	bool FaceEnabled;
	void FaceProducerTick();
private:
	void BeginDestroy() override;

	//Called on Input thread
	void OnTaskTick() override;
	void OnTaskInit() override;
	void OnTaskShutdown() override;

	//Scope locking
	FCriticalSection JointCriticalSection;
	FCriticalSection GestureCriticalSection;

	DataCollector *collector = nullptr;
};
