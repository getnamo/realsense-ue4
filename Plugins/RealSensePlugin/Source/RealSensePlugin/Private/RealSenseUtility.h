#pragma once

#include "pxcsensemanager.h"

//Conversion Utilities - Adjusted for UE orientation

FVector ueVectorFromRSVector(PXCPoint3DF32 in);

FRotator ueRotFromRSQuat(PXCPoint4DF32 rQuat);