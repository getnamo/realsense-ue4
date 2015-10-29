// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackgroundTask.generated.h"

class FBackgroundRunnable;
class UtilityTimer;

/**
 * 
 */
UCLASS()
class UBackgroundTask : public UObject
{
	GENERATED_BODY()

public:

	//Start/Stop Background Task - Called on Main Thread
	virtual void StartTask();
	virtual void EndTask();
	virtual void ConsumerTick(float DeltaTime){};	//optional, used for syncing

	//Data
	uint8* LatestData();

	//Convenience Callbacks - All run on game thread
	FGraphEventRef RunTaskOnGameThread(FSimpleDelegateGraphTask::FDelegate InFunction);
	FGraphEventRef RunLambdaOnGameThread(TFunction< void()> InFunction);

protected:
	void BeginDestroy() override;

	//Timer utility, used to re-schedule timer to arrive in sync with your tick
	UtilityTimer* funcTimer;
	int32 tickMs;

	//Background Task - Functions called on background task
	virtual void OnTaskTick();
	virtual void OnTaskInit(){};
	virtual void OnTaskShutdown(){};

	FBackgroundRunnable* Runnable;
};
