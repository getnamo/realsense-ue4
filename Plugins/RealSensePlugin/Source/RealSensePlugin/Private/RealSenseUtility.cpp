#include "RealSensePluginPrivatePCH.h"
#include "RealSenseUtility.h"

#define RS_TO_UE_SCALE 100

DEFINE_LOG_CATEGORY(RealSensePluginLog);

FVector ueVectorFromRSVector(PXCPoint3DF32 in)
{
	return FVector(-in.z, -in.x, in.y) * RS_TO_UE_SCALE;
}


FVector2D ueVector2DFromPXCPointF32(PXCPointF32 in)
{
	return FVector2D(in.x, in.y);
}

FRotator ueRotFromRSQuat(PXCPoint4DF32 rQuat)
{
	FQuat unconverted = FQuat(rQuat.x, rQuat.y, rQuat.z, rQuat.w);
	//Rotate the quaternion to UE space (applies R,Y,P order), P&Y axis need swapping
	//Equivalent of combining (rQuat, 0,90,-90) in blueprint
	return (FQuat(FRotator(0, 90, -90))*unconverted).Rotator();
}

FQuat ueQuatFromRSQuat(PXCPoint4DF32 rQuat)
{
	FQuat unconverted = FQuat(rQuat.x, rQuat.y, rQuat.z, rQuat.w);

	//Rotate the quaternion to UE space (applies R,Y,P order), P&Y axis need swapping
	//Equivalent of combining (rQuat, 0,90,-90) in blueprint
	return (FQuat(FRotator(0, 90, -90))*unconverted);
}

FMatrix ueMatrixFromRSMatrix(pxcF64* matrix)
{
	/*
	RS Basis depends on hand type with -z, -x, y as general format. This then needs to be inverted to point correctly (x = forward), which yields the matrix below.
	*/
	FVector inX, inY, inZ, inW;

	/*NB: this is what it should be if it wasn't a long array...
	inX.Set(matrix.zBasis.z, matrix.zBasis.x, -matrix.zBasis.y);
	inY.Set(-matrix.xBasis.z, -matrix.xBasis.x, matrix.xBasis.y);
	inZ.Set(-matrix.yBasis.z, -matrix.yBasis.x, matrix.yBasis.y);
	inW.Set(-matrix.origin.z, -matrix.origin.x, matrix.origin.y);
	*/

	inX.Set(matrix[8], matrix[6], -matrix[7]);
	inY.Set(-matrix[2], -matrix[0], matrix[1]);
	inZ.Set(-matrix[5], -matrix[3], matrix[4]);
	inW.Set(0, 0, 0);

	return (FMatrix(inX, inY, inZ, inW));
}

FIntRect ueRectFromRSRect(PXCRectI32 rsRect)
{
	FIntRect ueRect;
	ueRect.Min.X = rsRect.x;
	ueRect.Min.Y = rsRect.y;
	ueRect.Max.X = ueRect.Min.X + rsRect.w;
	ueRect.Max.Y = ueRect.Min.Y + rsRect.h;

	return ueRect;
}


FPIntRect uePRectFromRSRect(PXCRectI32 rect)
{
	return propertyRectFromIntRect(ueRectFromRSRect(rect));
}

FPIntRect propertyRectFromIntRect(FIntRect rect)
{
	FPIntRect fpRect;
	fpRect.Max = rect.Max;
	fpRect.Min = rect.Min;
	return fpRect;
}
