#include "RealSensePluginPrivatePCH.h"
#include "RealSenseInterface.h"


URealSenseInterface::URealSenseInterface(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

FString IRealSenseInterface::ToString()
{
	return "IRealSenseInterface::ToString()";
}