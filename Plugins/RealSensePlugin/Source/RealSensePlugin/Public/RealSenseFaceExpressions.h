#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseFaceExpression.h"
#include "RealSenseFaceExpressions.generated.h"

UCLASS(BlueprintType)
class URealSenseFaceExpressions : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	/**
	@brief Queries single expression result.
	@param[in] expression requested expression
	*/
	UFUNCTION(BlueprintCallable, Category = "RealSense Face Expressions")
	class URealSenseFaceExpression* QueryExpression(RSFaceExpression expression);

	void setFromRealSenseFaceExpressions(void* faceExpressionsPointer);

private:
	URealSenseFaceExpression* expression;
	void* expressionsPtr;
};