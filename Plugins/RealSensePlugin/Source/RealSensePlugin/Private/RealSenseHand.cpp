#include "RealSensePluginPrivatePCH.h"
#include "RealSenseUtility.h"
#include "RealSenseHand.h"
#include "RealSenseJoint.h"


URealSenseHand::URealSenseHand(const FObjectInitializer &init) : UObject(init)
{
	QueryJoint = init.CreateDefaultSubobject<URealSenseJoint>(this, TEXT("Joint"));
}

RealSenseBodySide handSideFromRealSenseBodySide(PXCHandData::BodySideType type){
	switch (type){
	case PXCHandData::BodySideType::BODY_SIDE_UNKNOWN:
		return BODY_SIDE_UNKNOWN;
		break;
	case PXCHandData::BodySideType::BODY_SIDE_LEFT:
		return BODY_SIDE_LEFT;
		break;
	case PXCHandData::BodySideType::BODY_SIDE_RIGHT:
		return BODY_SIDE_RIGHT;
		break;
	default:
		return BODY_SIDE_UNKNOWN;
	}
}

URealSenseJoint* URealSenseHand::QueryTrackedJoint(RealSenseJointType jointLabel)
{
	//update the query joint information and reutnr it
	PXCHandData::JointData rsJoint;
	PXCHandData::IHand* Hand = (PXCHandData::IHand*)RSHand;
	Hand->QueryTrackedJoint((PXCHandData::JointType)jointLabel, rsJoint);
	QueryJoint->setFromRealSenseJoint(&rsJoint);
	return QueryJoint;
}

void URealSenseHand::setFromRealSenseHand(void* HandPointer)
{
	RSHand = HandPointer;
	PXCHandData::IHand* Hand = (PXCHandData::IHand*)HandPointer;

	MassCenterWorld = ueVectorFromRSVector(Hand->QueryMassCenterWorld());
	BodySide = handSideFromRealSenseBodySide(Hand->QueryBodySide());
	PalmOrientation = ueRotFromRSQuat(Hand->QueryPalmOrientation());
	UserId = (int32)Hand->QueryUserId();
	UniqueId = (int32)Hand->QueryUniqueId();
	Openness = Hand->QueryOpenness();
}