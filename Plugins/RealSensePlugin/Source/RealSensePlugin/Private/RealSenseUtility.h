#pragma once

#include "pxcsensemanager.h"
#include "RealSenseTypes.h"

#define PLUGIN_VERSION "0.2.0"
#define RS_MAX_LANDMARK_POINTS 78

DECLARE_LOG_CATEGORY_EXTERN(RealSensePluginLog, Log, All);

//Measuring performance
class UtilityTimer
{
	int64 TickTime = 0;
	int64 TockTime = 0;
public:
	int64 unixTimeNow()
	{
		FDateTime timeUtc = FDateTime::UtcNow();
		return timeUtc.ToUnixTimestamp() * 1000 + timeUtc.GetMillisecond();
	}

	void tick()
	{
		TickTime = unixTimeNow();
	}

	int32 tock()
	{
		TockTime = unixTimeNow();
		return TockTime - TickTime;
	}
};


//Conversion Utilities - Adjusted for UE orientation

FVector ueVectorFromRSVector(PXCPoint3DF32 in);

FVector2D ueVector2DFromPXCPointF32(PXCPointF32 in);

FRotator ueRotFromRSQuat(PXCPoint4DF32 rQuat);

FQuat ueQuatFromRSQuat(PXCPoint4DF32 rQuat);

FMatrix ueMatrixFromRSMatrix(pxcF64* matrix);

FIntRect ueRectFromRSRect(PXCRectI32 rect);

FPIntRect uePRectFromRSRect(PXCRectI32 rect);

FPIntRect propertyRectFromIntRect(FIntRect rect);