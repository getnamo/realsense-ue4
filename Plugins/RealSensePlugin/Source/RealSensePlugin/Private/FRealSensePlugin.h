#pragma once

class DataCollector;
class URealSenseHandWorker;
class URealSenseFaceWorker;
//class RealSenseDelegate;

class FRealSensePlugin : public IRealSensePlugin
{
public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();

	/** Delegate Method To subscribe to event calls, only supports one listener for now */
	//void SetDelegate(RealSenseDelegate* newDelegate);
	//void RemoveDelegate();

	/** Manual looping, currently called in main thread */
	void RealSenseTick(float DeltaTime);
	void SetInterfaceDelegate(UObject* object);

private:
	//DataCollector *collector;
	URealSenseHandWorker* handWorker = nullptr;
	URealSenseFaceWorker* faceWorker = nullptr;

	void EnableHandDetection(bool enable);
	void EnableGestureDetection(bool enable, FString gesture);
	void EnableAllGestureDetection(bool enableAll);
	void EnableFaceDetection(bool enable);
	void EnableStreams();
};