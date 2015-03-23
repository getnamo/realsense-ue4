#include "RealSensePluginPrivatePCH.h"
#include "RealSenseInterface.h"


URealSenseInterface::URealSenseInterface(const FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

FString IRealSenseInterface::ToString()
{
	return "IRealSenseInterface::ToString()";
}