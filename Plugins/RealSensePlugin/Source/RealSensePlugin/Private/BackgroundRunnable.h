// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BackgroundTask.h"

typedef void(UBackgroundTask::*fptr)();

/**
 * 
 */
class FBackgroundRunnable : public FRunnable
{
public:
	//A background runnable that delegates running functions to another wrapper called background task
	FBackgroundRunnable(fptr InitCall, fptr RunCall, fptr StopCall, UBackgroundTask* Task);
	~FBackgroundRunnable();

	// Begin FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End FRunnable interface

private:
	fptr PrivateInitCall;
	fptr PrivateRunCall;
	fptr PrivateStopCall;

	FThreadSafeBool PrivateDone;
	UBackgroundTask* PrivateTask;	//reference to task holding you
	FRunnableThread* PrivateThread;
};
