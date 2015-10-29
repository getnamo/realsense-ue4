#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseFaceExpression.generated.h"

UENUM(BlueprintType)
enum RSFaceExpression
{
	EXPRESSION_BROW_RAISER_LEFT = 0,
	EXPRESSION_BROW_RAISER_RIGHT = 1,
	EXPRESSION_BROW_LOWERER_LEFT = 2,
	EXPRESSION_BROW_LOWERER_RIGHT = 3,

	EXPRESSION_SMILE = 4,
	EXPRESSION_KISS = 5,
	EXPRESSION_MOUTH_OPEN = 6,

	EXPRESSION_EYES_CLOSED_LEFT = 7,
	EXPRESSION_EYES_CLOSED_RIGHT = 8,

	/* Deprecated API */
	EXPRESSION_HEAD_TURN_LEFT = 9,
	EXPRESSION_HEAD_TURN_RIGHT = 10,
	EXPRESSION_HEAD_UP = 11,
	EXPRESSION_HEAD_DOWN = 12,
	EXPRESSION_HEAD_TILT_LEFT = 13,
	EXPRESSION_HEAD_TILT_RIGHT = 14,
	/* End Deprecated API */

	EXPRESSION_EYES_TURN_LEFT = 15,
	EXPRESSION_EYES_TURN_RIGHT = 16,
	EXPRESSION_EYES_UP = 17,
	EXPRESSION_EYES_DOWN = 18,
	EXPRESSION_TONGUE_OUT = 19,
	EXPRESSION_PUFF_RIGHT = 20,
	EXPRESSION_PUFF_LEFT = 21
};

UCLASS(BlueprintType)
class URealSenseFaceExpression : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Expression Property")
	TEnumAsByte<RSFaceExpression> Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RealSense Face Expression Property")
	int32 Intensity;

	void setFromRealSenseFaceExpression(void* faceExpressionPointer, RSFaceExpression passedType);

private:
};