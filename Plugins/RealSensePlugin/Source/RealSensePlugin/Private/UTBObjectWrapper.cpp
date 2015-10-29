// Fill out your copyright notice in the Description page of Project Settings.
#include "RealSensePluginPrivatePCH.h"
#include "UTBObjectWrapper.h"


#define NEXT_ENTRY_MAX 7

//Runnable

UTBObjectWrapper::UTBObjectWrapper()
{
	//Ensure init state
	for (int i = 0; i < 3; i++)
	{
		readyToRead[i] = false;
		readyToWrite[i] = true;
		data[i] = nullptr;
	}
	nextReadAddress = 0;
	nextWriteAddress = 0;
	lastReadAddress = 0;
}

/*struct FParameters
{
	uint8* Data;
};*/

int UTBObjectWrapper::nextEntryLoop(int current, FThreadSafeBool* boolList)
{
	//just so we don't crash too early if 2 threads are using it use 7 instead of 3
	if (nextEntryCounter == NEXT_ENTRY_MAX){
		//return an invalid index, allowing use to catch bad use cases
		return -1;
	}

	//Wrap-around at index = 2
	if (current > 1)
	{
		current = 0;
	}
	else{
		current++;
	}

	//grab atomic condition
	if (boolList[current] == true)
	{
		return current;
	}
	else
	{
		nextEntryCounter++;
		return nextEntryLoop(current, boolList);
	}
	return -1;
}

int UTBObjectWrapper::nextEntry(int current, FThreadSafeBool* boolList)
{
	
	nextEntryCounter = 0;
	int returnIndex = nextEntryLoop(current, boolList);

	//This is done to stop infinite looping, an index of -1 is invalid and you should debug
	if (returnIndex == -1)
		abort();
	else
		return returnIndex;
}

void UTBObjectWrapper::SetData(UObject* passedData)
{
	//atomic test
	if (readyToWrite[nextWriteAddress])
	{
		//This will lock the change so another producer thread won't crash it
		readyToWrite[nextWriteAddress] = false;

		//Set the data, duplicate the object, transient is ok
		data[nextWriteAddress] = DuplicateObject(passedData, nullptr);	//TODO: root objects so we don't get GC problems with reading.
																		//TODO: optimize so we don't recreate objects that aren't being read, duplicate only when needed.

		//Enable reading and writing on this one
		readyToRead[nextWriteAddress] = true;
		readyToWrite[nextWriteAddress] = true;

		//set the next read address (this one)
		nextReadAddress = nextWriteAddress;

		//set the next write address (next one)
		nextWriteAddress = nextEntry(nextWriteAddress, readyToWrite);
	}
	else
	{
		//This entry wasn't writable, try another one. If you've been locked out you should always hit the other free one
		nextWriteAddress = nextEntry(nextWriteAddress, readyToWrite);
		SetData(passedData);
	}
}


UObject* UTBObjectWrapper::ReadData()
{
	if (readyToRead[nextReadAddress])
	{
		//Disable reading/writing on this one as it is being read
		readyToRead[nextReadAddress] = false;
		readyToWrite[nextReadAddress] = false;

		//Re-enable reading and writing on old one
		readyToWrite[lastReadAddress] = true;
		readyToRead[lastReadAddress] = true;

		//Set our new last read
		lastReadAddress = nextReadAddress;

		return data[nextReadAddress];
	}
	else
	{
		nextReadAddress = nextEntry(nextReadAddress, readyToRead);
		return ReadData();
	}
}


// Lock free list.
/*TLockFreePointerList<FParameters> GMyGlobalParameters;

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
