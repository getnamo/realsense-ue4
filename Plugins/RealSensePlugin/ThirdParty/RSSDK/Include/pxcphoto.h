/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2015 Intel Corporation. All Rights Reserved.

*******************************************************************************/
/** @file pxcphoto.h
    Defines the PXCPhoto interface, which defines the photo container.
  */
#pragma once
#include "pxccapture.h"
#include "pxcaddref.h"

/**
    This interface defines the photo container. Call PXCSession::CreatePhoto to create
	an instance of this object. Then initiailize it with different member functions.

	The interface extends PXCMetadata. Use QueryInstance<PXCMetadata>() to access
	the PXCMetadata features.
*/

class PXCPhoto: public PXCBase {
public:

    PXC_CUID_OVERWRITE(PXC_UID('G','D','V','2'));

    /** 
        @brief Import the preview sample content into the photo instance.
        @param[in]  sample				The PXCCapture::Sample instance from the SenseManager QuerySample().
        @return PXC_STATUS_NO_ERROR     Successful execution.
    */
    virtual pxcStatus PXCAPI ImportFromPreviewSample(PXCCapture::Sample *sample)=0;

    /** 
        @brief Import the photo content from the Google* Depth File Format v2.0.
        @param[in]   filename           The file name.
        @return PXC_STATUS_NO_ERROR     Successful execution.
    */ 
    virtual pxcStatus PXCAPI LoadXDM(pxcCHAR *filename)=0;

    /** 
        @brief Export the photo content to the Google Depth File Format v2.0.
		@param[in]   filename           The file name.
		@return PXC_STATUS_NO_ERROR     Successful execution.
    */ 
    virtual pxcStatus PXCAPI SaveXDM(pxcCHAR *filename)=0;

	/**
	@brief Copy the content from the source photo
	@param[in]  photo	The source photo.
	@return PXC_STATUS_NO_ERROR	Successful execution.
	*/
	virtual pxcStatus PXCAPI CopyPhoto(const PXCPhoto *photo) = 0;

	/**
	@brief Get the reference image of the photo. The reference image is usually the processed color image.
	@return The PXCImage instance.
	*/
	virtual PXCImage* PXCAPI QueryReferenceImage(void) = 0;

	/**
	@brief Get the original image of the photo. The original image is usually the unprocessed color image.
	@return The PXCImage instance.
	*/
	virtual PXCImage* PXCAPI QueryOriginalImage(void) = 0;
	
	/**
	@brief Get the depth image of the photo. this would be the processed depth if it undergoes processing.
	@return The PXCImage instance.
	*/
	virtual PXCImage* PXCAPI QueryDepthImage(void) = 0;
	
	/**
	@brief Get the raw depth image of the photo. This would be the unprocessed depth captured from the camera or loaded from a file if it existed.
	@return The PXCImage instance.
	*/
	virtual PXCImage* PXCAPI QueryRawDepthImage(void) = 0;

	/**
	@brief Increase a reference count of the sample.
	*/
	__inline void AddRef(void) {
		QueryInstance<PXCAddRef>()->AddRef();
	}
};
