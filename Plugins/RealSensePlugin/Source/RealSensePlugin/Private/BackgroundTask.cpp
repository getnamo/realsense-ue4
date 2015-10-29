// Fill out your copyright notice in the Description page of Project Settings.
#include "RealSensePluginPrivatePCH.h"
#include "BackgroundTask.h"
#include "RealSenseUtility.h"
#include "BackgroundRunnable.h"

//Runnable


void UBackgroundTask::StartTask()
{
	//Already started
	if (Runnable != nullptr)
		return;
	
	funcTimer = new UtilityTimer;
	//Set the runnable function delegates and starts the thread
	Runnable = new FBackgroundRunnable(&UBackgroundTask::OnTaskInit, &UBackgroundTask::OnTaskTick, &UBackgroundTask::OnTaskShutdown, this);
}

void UBackgroundTask::EndTask()
{
	if (Runnable == nullptr)
		return;

	//Stop the thread by command
	Runnable->Stop();

	delete funcTimer;
	delete Runnable;
	Runnable = nullptr;
}

FGraphEventRef UBackgroundTask::RunTaskOnGameThread(FSimpleDelegateGraphTask::FDelegate InFunction)
{
	return FSimpleDelegateGraphTask::CreateAndDispatchWhenReady
		(
		InFunction
		, TStatId()
		, nullptr
		, ENamedThreads::GameThread
		);
}

FGraphEventRef UBackgroundTask::RunLambdaOnGameThread(TFunction< void()> InFunction)
{
	return FFunctionGraphTask::CreateAndDispatchWhenReady(InFunction, TStatId(), nullptr, ENamedThreads::GameThread);
}

//Ensuring we close the thread if we get deleted
void UBackgroundTask::BeginDestroy()
{
	UObject::BeginDestroy();

	if (Runnable!=nullptr)
		Runnable->Stop();
}

void UBackgroundTask::OnTaskTick()
{
	tickMs = funcTimer->tock();
	funcTimer->tick();
}

//Below -- Example code only for now

/*
struct FParameters
{
	uint8* Data;
};

// Task graph.
void SomeFunctionThatWillBeCalledOnGameThread(FParameters* Params)
{
	// Parse the parameters or copy for later usage.
}

void CalledOnBackgroundThread_TG()
{
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady
		(
		FSimpleDelegateGraphTask::FDelegate::CreateStatic(&SomeFunctionThatWillBeCalledOnGameThread, new FParameters())
		, TStatId() //TEXT("PassParamsToGrameThread")
		, nullptr
		, ENamedThreads::GameThread
		);
}

// Lock free list.
TLockFreePointerList<FParameters> GMyGlobalParameters;

void ThisIsTickedOnTheMainThread()
{
	// Grab all parameters.
	TArray<FParameters*> MyParams;
	GMyGlobalParameters.PopAll(MyParams);
}

void CalledOnBackgroundThread_LFL()
{
	// Enqueue my parameters. 
	GMyGlobalParameters.Push(new FParameters());
}*/


/*void awesomeLambdaDelegate()
{
	if (FTaskGraphInterface::IsRunning())
	{
		FGraphEventRef SomeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			//FCoreDelegates::ApplicationWillTerminateDelegate.Broadcast();
		}, TStatId(), NULL, ENamedThreads::GameThread);
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(SomeTask);
	}
}*/



/**FAutoDeleteAsyncTask - template task for jobs that delete themselves when complete
Sample code:
class ExampleAutoDeleteAsyncTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<ExampleAutoDeleteAsyncTask>;
	int32 ExampleData;
	ExampleAutoDeleteAsyncTask(int32 InExampleData)
	: ExampleData(InExampleData)
	{
	}
	void DoWork()
	{
		... do the work here
	}
	FORCEINLINE TStatId GetStatId() const
	{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ExampleAutoDeleteAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};

start an example job
(new FAutoDeleteAsyncTask<ExampleAutoDeleteAsyncTask>(5)->StartBackgroundTask();
do an example job now, on this thread
(new FAutoDeleteAsyncTask<ExampleAutoDeleteAsyncTask>(5)->StartSynchronousTask();
**/