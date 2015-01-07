#pragma once

class DataCollector;
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
	DataCollector *collector;

	void EnableHandDetection();
	void EnableGestureDetection(bool enableAll);
	void EnableFaceDetection();
};