 
#pragma once

#include "RealSenseHand.h"
#include "RealSenseFace.h"
#include "RealSenseFaceLandmarkPoint.h"
#include "RealSenseInterface.generated.h"


UINTERFACE(MinimalAPI)
class URealSenseInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IRealSenseInterface
{
	GENERATED_IINTERFACE_BODY()

public:

	//Hand
	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Event")
	void HandMoved(URealSenseHand* hand);

	//Joint
	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Event")
	void JointMoved(URealSenseJoint* joint);

	//Face
	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Event")
	void FaceDetected(URealSenseFace* face);

	//Landmarks
	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Event")
	void FaceLandmarkMoved(URealSenseFaceLandmarkPoint* point);

	//Expressions
	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Event")
	void FaceExpressionsDetected(URealSenseFaceExpressions* expressions);

	//Gestures
	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Gesture Event")
	void SpreadFingersGestureDetected(URealSenseHand* hand);

	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Gesture Event")
	void FistGestureDetected(URealSenseHand* hand);

	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Gesture Event")
	void TapGestureDetected(URealSenseHand* hand);

	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Gesture Event")
	void ThumbUpGestureDetected(URealSenseHand* hand);

	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Gesture Event")
	void ThumbDownGestureDetected(URealSenseHand* hand);

	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Gesture Event")
	void TwoFingersPinchOpenGestureDetected(URealSenseHand* hand);

	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Gesture Event")
	void VSignGestureDetected(URealSenseHand* hand);

	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Gesture Event")
	void FullPinchGestureDetected(URealSenseHand* hand);

	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Gesture Event")
	void SwipeGestureDetected(URealSenseHand* hand);

	UFUNCTION(BlueprintImplementableEvent, Category = "RealSense Interface Gesture Event")
	void WaveGestureDetected(URealSenseHand* hand);



	//Help identifying
	virtual FString ToString();
};