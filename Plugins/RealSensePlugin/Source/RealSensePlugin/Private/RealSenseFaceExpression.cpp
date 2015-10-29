#include "RealSensePluginPrivatePCH.h"
#include "RealSenseFaceExpression.h"
#include "pxcsensemanager.h"
#include "pxcfacedata.h"
#include "RealSenseUtility.h"

URealSenseFaceExpression::URealSenseFaceExpression(const FObjectInitializer &init) : UObject(init)
{

}

void URealSenseFaceExpression::setFromRealSenseFaceExpression(void* faceExpressionPointer, RSFaceExpression passedType)
{
	PXCFaceData::ExpressionsData::FaceExpressionResult* expressions = (PXCFaceData::ExpressionsData::FaceExpressionResult*)faceExpressionPointer;

	Intensity = expressions->intensity;
	Type = passedType;
}