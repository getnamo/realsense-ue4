// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

/**
 * The public interface to this module
 */
class IRealSensePlugin : public IModuleInterface
{

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IRealSensePlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< IRealSensePlugin >("RealSensePlugin");
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("RealSensePlugin");
	}

	/**
	 * Public API, implemented in FRealSensePlugin.cpp
	 * Optional way of getting data is to subscribe to the RealSenseDelegate class through inheritance
	 */
	virtual void RealSenseTick(float DeltaTime){};
	virtual void SetInterfaceDelegate(UObject* object){};

	virtual void EnableGestureDetection(bool enable, FString gesture){};
	virtual void EnableAllGestureDetection(bool enableAll){};
	virtual void EnableHandDetection(bool enable){};
	virtual void EnableFaceDetection(bool enable){};
};

