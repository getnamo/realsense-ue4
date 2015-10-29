#include "RealSensePluginPrivatePCH.h"
#include "RealSenseFace.h"
#include "pxcsensemanager.h"
#include "pxcfacedata.h"
#include "RealSenseUtility.h"
#include "RealSenseFaceDetection.h"
#include "RealSenseFaceExpressions.h"
#include "RealSenseFaceLandmarks.h"
#include "RealSenseFacePose.h"
#include "RealSenseFacePulse.h"
#include "RealSenseFaceRecognition.h"

URealSenseFace::URealSenseFace(const FObjectInitializer &init) : UObject(init)
{
	detection = init.CreateDefaultSubobject<URealSenseFaceDetection>(this, TEXT("detection"));
	landmarks = init.CreateDefaultSubobject<URealSenseFaceLandmarks>(this, TEXT("landmarks"));
	pose = init.CreateDefaultSubobject<URealSenseFacePose>(this, TEXT("pose"));
	expressions = init.CreateDefaultSubobject<URealSenseFaceExpressions>(this, TEXT("expressions"));
	recognition = init.CreateDefaultSubobject<URealSenseFaceRecognition>(this, TEXT("recognition"));
	pulse = init.CreateDefaultSubobject<URealSenseFacePulse>(this, TEXT("pulse"));
}


URealSenseFaceDetection* URealSenseFace::QueryDetection()
{
	PXCFaceData::Face* face = (PXCFaceData::Face*)facePtr;
	detection->setFromRealSenseDetection(face);
	return detection;
}


URealSenseFaceLandmarks*  URealSenseFace::QueryLandmarks()
{
	PXCFaceData::Face* face = (PXCFaceData::Face*)facePtr;
	landmarks->setFromRealSenseLandmark(face->QueryLandmarks());
	return landmarks;
}


URealSenseFacePose* URealSenseFace::QueryPose()
{
	PXCFaceData::Face* face = (PXCFaceData::Face*)facePtr;
	pose->setFromRealSenseFacePose(face->QueryPose());
	return pose;
}


URealSenseFaceExpressions* URealSenseFace::QueryExpressions()
{
	PXCFaceData::Face* face = (PXCFaceData::Face*)facePtr;
	expressions->setFromRealSenseFaceExpressions(face->QueryExpressions());
	return expressions;
}


URealSenseFaceRecognition* URealSenseFace::QueryRecognition()
{
	PXCFaceData::Face* face = (PXCFaceData::Face*)facePtr;
	recognition->setFromRealSenseFaceRecognition(face->QueryRecognition());
	return recognition;
}


URealSenseFacePulse* URealSenseFace::QueryPulse()
{
	PXCFaceData::Face* face = (PXCFaceData::Face*)facePtr;
	pulse->setFromRealSenseFacePulse(face->QueryPulse());
	return pulse;
}

void URealSenseFace::setFromRealSenseFace(void* pointer)
{
	facePtr = pointer;
	PXCFaceData::Face* face = (PXCFaceData::Face*)facePtr;

	UserID = face->QueryUserID();
}