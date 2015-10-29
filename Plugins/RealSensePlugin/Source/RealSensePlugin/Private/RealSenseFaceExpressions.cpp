#include "RealSensePluginPrivatePCH.h"
#include "RealSenseFaceExpressions.h"
#include "pxcsensemanager.h"
#include "pxcfacedata.h"
#include "RealSenseUtility.h"

URealSenseFaceExpressions::URealSenseFaceExpressions(const FObjectInitializer &init) : UObject(init)
{
	expression = init.CreateDefaultSubobject<URealSenseFaceExpression>(this, TEXT("expression"));
}


URealSenseFaceExpression* URealSenseFaceExpressions::QueryExpression(RSFaceExpression expressionEnum)
{
	PXCFaceData::ExpressionsData* expressions = (PXCFaceData::ExpressionsData*)expressionsPtr;
	PXCFaceData::ExpressionsData::FaceExpressionResult result;
	expressions->QueryExpression((PXCFaceData::ExpressionsData::FaceExpression)expressionEnum, &result);
	expression->setFromRealSenseFaceExpression(&result, expressionEnum);
	return expression;
}

void URealSenseFaceExpressions::setFromRealSenseFaceExpressions(void* faceExpressionsPointer)
{
	expressionsPtr = faceExpressionsPointer;
	PXCFaceData::ExpressionsData* expressions = (PXCFaceData::ExpressionsData*)expressionsPtr;
}