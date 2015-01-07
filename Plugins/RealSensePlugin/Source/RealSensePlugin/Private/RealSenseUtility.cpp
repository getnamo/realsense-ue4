#include "RealSensePluginPrivatePCH.h"
#include "RealSenseUtility.h"

#define RS_TO_UE_SCALE 100

FVector ueVectorFromRSVector(PXCPoint3DF32 in)
{
	return FVector(-in.z, -in.x, in.y) * RS_TO_UE_SCALE;
}

FRotator ueRotFromRSQuat(PXCPoint4DF32 rQuat)
{
	FQuat unconverted = FQuat(rQuat.x, rQuat.y, rQuat.z, rQuat.w);
	//Rotate the quaternion to UE space (applies R,Y,P order), P&Y axis need swapping
	//Equivalent of combining (rQuat, 0,90,-90) in blueprint
	return (FQuat(FRotator(0, 90, -90))*unconverted).Rotator();
}