// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UTBObjectWrapper.generated.h"

class FBackgroundRunnable;

/**
 * 
 */
UCLASS()
class UTBObjectWrapper : public UObject
{
	GENERATED_BODY()
private:
	UObject* data[3];
	int nextWriteAddress;
	int nextReadAddress;
	int lastReadAddress;
	FThreadSafeBool readyToRead[3];
	FThreadSafeBool readyToWrite[3];

	int nextEntryCounter = 0;
	int nextEntryLoop(int current, FThreadSafeBool* boolList);
	int nextEntry(int current, FThreadSafeBool* boolList);

public:
	UTBObjectWrapper();

	//Called on the producer thread, safe to call one at a time, will cause DuplicateObject call on the parameter
	void SetData(UObject* data);

	//Called on the consumer thread, safe to call one at a time, if a second call comes
	UObject* ReadData();
};
