// Fill out your copyright notice in the Description page of Project Settings.
#include "RealSensePluginPrivatePCH.h"
#include "BackgroundRunnable.h"
#include "RealSenseUtility.h"

FBackgroundRunnable::FBackgroundRunnable(fptr InitCall, fptr RunCall, fptr StopCall, UBackgroundTask* Task)
{
	PrivateInitCall = InitCall;
	PrivateRunCall = RunCall;
	PrivateStopCall = StopCall;
	PrivateTask = Task;
	PrivateDone = false;

	//windows default = 8mb for thread, could specify more
	PrivateThread = FRunnableThread::Create(this, TEXT("FBackgroundRunnableThread"), 0, TPri_Normal);	//Try TPri_BelowNormal if we're struggling
}

FBackgroundRunnable::~FBackgroundRunnable()
{
	Stop();

	//delete PrivateThread;
	//PrivateThread = NULL;
}

uint32 FBackgroundRunnable::Run()
{	
	//(PrivateTask->*PrivateRunCall)();

	//Start snooze
	FPlatformProcess::Sleep(0.01f);

	//For now test that we're getting shit from another thread...
	while (!PrivateDone)
	{
		//UE_LOG(RealSensePluginLog, Log, TEXT("tick"));

		//Spin!
		(PrivateTask->*PrivateRunCall)();
	}

	return 0;
}

void someFunction()
{
	UE_LOG(RealSensePluginLog, Log, TEXT("BackgroundThread with 2ID:"));
}

bool FBackgroundRunnable::Init()
{
	PrivateTask->RunTaskOnGameThread(FSimpleDelegateGraphTask::FDelegate::CreateStatic(&someFunction));
	
	(PrivateTask->*PrivateInitCall)();
	return true;
}

void FBackgroundRunnable::Stop()
{
	UE_LOG(RealSensePluginLog, Log, TEXT("FBackgroundRunnable::Stop called."));
	PrivateDone = true;
	(PrivateTask->*PrivateStopCall)();
}