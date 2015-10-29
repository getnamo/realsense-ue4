#include "RealSensePluginPrivatePCH.h"
#include "RealSenseFacePose.h"
#include "pxcsensemanager.h"
#include "pxcfacedata.h"
#include "RealSenseUtility.h"

URealSenseFacePose::URealSenseFacePose(const FObjectInitializer &init) : UObject(init)
{
}


FMatrix URealSenseFacePose::RotationMatrix()
{
	//Rotation matrix is rarely needed so we're making it a function instead

	PXCFaceData::PoseData* pose = (PXCFaceData::PoseData*)facePosePtr;
	pxcF64 matrix[9];
	pose->QueryRotationMatrix(matrix);
	return ueMatrixFromRSMatrix(matrix);
}

void URealSenseFacePose::setFromRealSenseFacePose(void* facePosePointer)
{
	facePosePtr = facePosePointer;
	if (facePosePtr == nullptr)
		return;
	PXCFaceData::PoseData* pose = (PXCFaceData::PoseData*)facePosePtr;

	Confidence = pose->QueryConfidence();
	
	//Face Orientation
	PXCFaceData::PoseQuaternion poseQuat;
	pose->QueryPoseQuaternion(&poseQuat);

	Quaternion = FQuat(poseQuat.x, poseQuat.y, poseQuat.z, poseQuat.w);
	Rotation = Quaternion.Rotator();

	//Head
	PXCFaceData::HeadPosition rsHeadPosition;
	pose->QueryHeadPosition(&rsHeadPosition);
	HeadConfidence = rsHeadPosition.confidence;

	PXCPoint3DF32 point;
	point.x = rsHeadPosition.headCenter.x;
	point.y = rsHeadPosition.headCenter.y;
	point.z = rsHeadPosition.headCenter.z;
	HeadPosition = ueVectorFromRSVector(point);

}