#pragma once
#include "RealSensePluginPublicPCH.h"
#include "RealSenseTypes.generated.h"

//not currently utilized, to be extended
USTRUCT()
struct FPIntRect
{
	GENERATED_USTRUCT_BODY()
	FIntPoint Min;
	FIntPoint Max;
};
