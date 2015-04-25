/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2014 Intel Corporation. All Rights Reserved.

*******************************************************************************/
/** @file PXCEnhancedPhotography.h
    Defines the PXCEnhancedPhotography interface, which programs may use to process snapshots of captured frames
	to measure distance and refocus the image
 */
#pragma once
#include "pxccapture.h"

/**
This class defines a standard interface for enhanced photography algorithms.
*/
class PXCEnhancedPhotography:public PXCBase {
public:

	PXC_CUID_OVERWRITE(PXC_UID('E','P','I','N'));

	/* 
	MeasureDistance: measure the distance between 2 points in mm
	sample: is the color and depth sample
	startPoint, endPoint: are the start pont and end point of the distance that need to be measured.
	radius1 and radius2: are reserved paramaeters for future developement
	Note: depth data must be availible and acurate at the start and end point selected. 
	*/
	virtual pxcF32 PXCAPI MeasureDistance(const PXCCapture::Sample *sample, PXCPointI32 startPoint, PXCPointI32 endPoint, pxcI32 radius1, pxcI32 radius2) = 0;

	/*
	MeasureDistance: measure the distance between 2 points in mm
	sample: is the color and depth sample
	startPoint, endPoint: are the start pont and end point of the distance that need to be measured.
	Note: depth data must be availible and acurate at the start and end point selected.
	*/
	__inline pxcF32 MeasureDistance(const PXCCapture::Sample *sample, PXCPointI32 startPoint, PXCPointI32 endPoint) { 
		return MeasureDistance(sample, startPoint, endPoint, 0, 0);
	}
	
	// refocussing features types available: Exponential , Cylinder , pseudocylinderSync , Gaussian
	enum BlurType {
		BLUR_EXPONENTIAL = 0, 
		BLUR_CYLINDER_BLUR, 
		BLUR_PSEUDO_CYLINDER_SYNC, 
		BLUR_GAUSSIAN,
	};
	
	/* 
	DepthRefocus: refocus the image at input focusPoint by using depth data refocus
	sample: is the color and depth sample
	focusPoint: is the selected point foir refocussing.
	aperture: Range of the blur area [1-100]
	BlurType: Refocuse types available: Exponential , Cylinder , pseudocylinderSync , Gaussian   
	Note: The application must release the returned refocussed image
	*/
	virtual PXCImage* PXCAPI DepthRefocus(const PXCCapture::Sample *sample, PXCPointI32 focusPoint, pxcF32 aperture, BlurType blurtype) = 0;

	/*
	DepthRefocus: refocus the image at input focusPoint by using depth data refocus
	sample: is the color and depth sample
	focusPoint: is the selected point for refocussing.
	aperture: Range of the blur area [1-100]
	Note: The application must release the returned refocussed image
	*/
	__inline PXCImage* DepthRefocus(const PXCCapture::Sample *sample, PXCPointI32 focusPoint, pxcF32 aperture) { 
		return DepthRefocus(sample, focusPoint, aperture, PXCEnhancedPhotography::BLUR_EXPONENTIAL);
	}

	/*
	DepthRefocus: refocus the image at input focusPoint by using depth data refocus
	sample: is the color and depth sample
	focusPoint: is the selected point foir refocussing.
	Note: The application must release the returned refocussed image
	*/
	__inline PXCImage* DepthRefocus(const PXCCapture::Sample *sample, PXCPointI32 focusPoint) { 
		return DepthRefocus(sample, focusPoint, 50.0, PXCEnhancedPhotography::BLUR_EXPONENTIAL);
	}
};