/** @file PXCHumanTrackingModule.h
    Defines the PXCHumanTrackingModule interface, which gives access the human human module's configuration and output data.
 */
#pragma once
#include "pxcbase.h"


class PXCHumanTrackingConfiguration;
class PXCHumanTrackingData;


/**
	@Class PXChumanTrackingModule 
*/
class PXCHumanTrackingModule : public PXCBase 
{
public:

	PXC_CUID_OVERWRITE(PXC_UID('P','O','T','M'));

	/** 
		@brief returns current configuraation.
	*/
	virtual PXCHumanTrackingConfiguration* PXCAPI QueryConfiguration() = 0;

	/** 
		@brief returns latest available output data.
	*/
	virtual PXCHumanTrackingData* PXCAPI QueryOutput() = 0;
};
