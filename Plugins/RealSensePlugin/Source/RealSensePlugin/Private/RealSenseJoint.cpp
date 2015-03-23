#include "RealSensePluginPrivatePCH.h"
#include "RealSenseUtility.h"
#include "RealSenseJoint.h"


URealSenseJoint::URealSenseJoint(const FObjectInitializer &init) : UObject(init)
{
}

void URealSenseJoint::setFromRealSenseJoint(void* jointPointer)
{
	PXCHandData::JointData* joint = (PXCHandData::JointData*)jointPointer;
	Confidence = joint->confidence;
	PositionWorld = ueVectorFromRSVector(joint->positionWorld);
	PositionImage = ueVectorFromRSVector(joint->positionImage);
}