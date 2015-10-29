#include "RealSensePluginPrivatePCH.h"
#include "RealSenseFacePulse.h"
#include "pxcsensemanager.h"
#include "pxcfacedata.h"
#include "RealSenseUtility.h"

URealSenseFacePulse::URealSenseFacePulse(const FObjectInitializer &init) : UObject(init)
{
}


void URealSenseFacePulse::setFromRealSenseFacePulse(void* facePulsePointer)
{
	PXCFaceData::PulseData* pulse = (PXCFaceData::PulseData*)facePulsePointer;

	HeartRate = pulse->QueryHeartRate();
}