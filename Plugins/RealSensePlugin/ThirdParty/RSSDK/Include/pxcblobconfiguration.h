/******************************************************************************
	INTEL CORPORATION PROPRIETARY INFORMATION
	This software is supplied under the terms of a license agreement or nondisclosure
	agreement with Intel Corporation and may not be copied or disclosed except in
	accordance with the terms of that agreement
	Copyright(c) 2011-2015 Intel Corporation. All Rights Reserved.
*******************************************************************************/
#pragma once
#include "pxcbase.h"
#include "pxcdefs.h"

/**
	@class PXCBlobConfiguration
	@brief Retrieve the current configuration of the blob module and set new configuration values.
	@note Changes to PXCBlobConfiguration are applied only when ApplyChanges() is called.
*/
class PXCBlobConfiguration: public PXCBase
{
public:

	/* Constants */
	PXC_CUID_OVERWRITE(PXC_UID('B','M','C','G'));

public:

	/* General */

	/**
		@brief Apply the configuration changes to the blob module.
		This method must be called in order for any configuration changes to apply.
        @return PXC_STATUS_NO_ERROR - successful operation.
        @return PXC_STATUS_DATA_NOT_INITIALIZED - the configuration was not initialized.                        
	*/
	virtual pxcStatus PXCAPI ApplyChanges() = 0;

	/**  
		@brief Restore configuration settings to the default values.
        @return PXC_STATUS_NO_ERROR - successful operation.
        @return PXC_STATUS_DATA_NOT_INITIALIZED - the configuration was not initialized.                  
	*/
	virtual pxcStatus PXCAPI RestoreDefaults() = 0;

	/**
		@brief Retrieve the blob module's current configuration settings.
        @return PXC_STATUS_NO_ERROR - successful operation.
        @return PXC_STATUS_DATA_NOT_INITIALIZED - failed to retrieve current configuration.
	*/
	virtual pxcStatus PXCAPI Update() = 0;

	/* Configuration */
	
   	/**
		@brief Set the smoothing strength for the segmentation image.
		@param[in] smoothingValue - a value between 0 (no smoothing) to 1 (very smooth).
		@return PXC_STATUS_NO_ERROR - successful operation.
		@return PXC_STATUS_PARAM_UNSUPPORTED - invalid smoothing value.
		@see QuerySegmentationSmoothing
	*/
	virtual pxcStatus PXCAPI SetSegmentationSmoothing(pxcF32 smoothingValue) = 0;

	/**
		@brief Get the segmentation image smoothing value.
		@return The segmentation image smoothing value.
		@see SetSegmentationSmoothing
	*/
	virtual pxcF32  PXCAPI QuerySegmentationSmoothing() = 0;
	
	
	/**
		@brief Sets the contour smoothing value.
		@param[in] smoothingValue - a value between 0 (no smoothing) to 1 (very smooth).
		@return PXC_STATUS_NO_ERROR - successful operation.
		@return PXC_STATUS_PARAM_UNSUPPORTED - invalid smoothing value.
		@see QueryContourSmoothing
	*/
	virtual pxcStatus PXCAPI SetContourSmoothing(pxcF32 smoothingValue) = 0;

	/**
		@brief Get the contour smoothing value.
		@return The contour smoothing value.
		@see SetContourSmoothing
	*/
	virtual pxcF32  PXCAPI QueryContourSmoothing() = 0;
		
	/** 
		@brief Set the maximal number of blobs that can be detected (default is 1).
		@param[in] maxBlobs - the maximal number of blobs that can be detected (limited to 4).
		@return PXC_STATUS_NO_ERROR - successful operation.
		@return PXC_STATUS_PARAM_UNSUPPORTED - invalid maxBlobs value (in this case, the last valid value will be retained).
		@see QueryMaxBlobs
	*/
	virtual pxcStatus PXCAPI SetMaxBlobs(const pxcI32 maxBlobs) = 0;

	/**
		@brief Get the maximal number of blobs that can be detected.
		@return The maximal number of blobs that can be detected.
		@see SetMaxBlobs
	*/
	virtual pxcI32 PXCAPI QueryMaxBlobs(void) const = 0;	
	
	/** 
		@brief Set the maximal distance in meters of a detected blob from the sensor. 
		Only objects that are within this limit will be identified as blobs.
		@param[in] maxDistance - the maximal distance in meters of a blob from the sensor
		@return PXC_STATUS_NO_ERROR - successful operation
		@return PXC_STATUS_PARAM_UNSUPPORTED - invalid maxDistance value (in this case, the last valid value will be retained).
		@see QueryMaxDistance
	*/
	virtual pxcStatus PXCAPI SetMaxDistance(pxcF32 maxDistance) = 0;

	/** 
		@brief Get the maximal distance in meters of a detected blob from the sensor. 
		@return The maximal distance of a detected blob from the sensor.
		@see SetMaxDistance
	*/
	virtual pxcF32 PXCAPI QueryMaxDistance(void) const = 0;
	
	/**
		@brief Set the maximal depth in meters of a blob (maximal distance between closest and farthest points in the blob).
		@param[in] maxDepth - the maximal depth in meters of the blob.
		@return PXC_STATUS_NO_ERROR - successful operation.
		@return PXC_STATUS_PARAM_UNSUPPORTED - invalid maxDepth value (in this case, the last valid value will be retained).
		@see QueryMaxObjectDepth
	*/
	virtual pxcStatus PXCAPI SetMaxObjectDepth(pxcF32 maxDepth) = 0;

	/** 
		@brief Get the maximal depth in meters of a blob.
		@return The maximal depth in meters of a blob.
		@see SetMaxObjectDepth
	*/
	virtual pxcF32 PXCAPI QueryMaxObjectDepth(void) const = 0;	
	
	/** 
		@brief Set the minimal blob size in pixels.
		Only objects that are larger than this size are identified as blobs.
		@param[in] minBlobSize - the minimal blob size in pixels (cannot be more than a quarter of the image size in pixels).
		@return PXC_STATUS_NO_ERROR - successful operation.
		@return PXC_STATUS_PARAM_UNSUPPORTED - invalid minBlobSize value (in this case, the last valid value will be retained).
		@see QueryMinPixelCount
	*/
	virtual pxcStatus PXCAPI SetMinPixelCount(pxcI32 minBlobSize) = 0;

	/** 
		@brief Get the minimal blob size in pixels.
		@return The minimal blob size in pixels.
		@see SetMinPixelCount
	*/
	virtual pxcI32 PXCAPI QueryMinPixelCount(void) const = 0;	
	
	/**
		@brief Enable extraction of the segmentation image.
		@param[in] enableFlag - set to true if the segmentation image should be extracted; otherwise set to false. 
		@return PXC_STATUS_NO_ERROR - successful operation.
		@see IsSegmentationImageEnabled
	*/
	virtual pxcStatus PXCAPI EnableSegmentationImage(pxcBool enableFlag) = 0;

	/**
	@brief Return the segmentation image extraction flag.
	@return The segmentation image extraction flag.
	@see EnableSegmentationImage
	*/
	virtual pxcBool  PXCAPI IsSegmentationImageEnabled() = 0;
	
	/**
		@brief Enable extraction of the contour data.
		@param[in] enableFlag - set to true if contours should be extracted; otherwise set to false. 
		@return PXC_STATUS_NO_ERROR - successful operation.
		@see IsContourExtractionEnabled
	*/
	virtual pxcStatus PXCAPI EnableContourExtraction(pxcBool enableFlag) = 0;

	/**
	@brief Return the contour extraction flag.
	@return The contour extraction flag.
	@see EnableContourExtraction
	*/
	virtual pxcBool  PXCAPI IsContourExtractionEnabled() = 0;	

	/**
		@brief Enable or disable the stabilization feature.\n
		
		Enabling this feature produces smoother tracking of the extremity points, ignoring small shifts and "jitters".\n
		@Note: in some cases the tracking may be less sensitive to minor movements and some blob pixels may be outside of the extremities. 
		
		@param[in] enableFlag - true to enable the stabilization; false to disable it.
		@return PXC_STATUS_NO_ERROR - operation succeeded. 
	*/
	virtual pxcStatus PXCAPI EnableStabilizer(pxcBool enableFlag)= 0;	

	/**
	@brief Return blob stabilizer activation status.
	@return true if blob stabilizer is enabled, false otherwise.
	*/
	virtual pxcBool PXCAPI IsStabilizerEnabled()= 0;
	
	/** 
		@brief Set the minimal contour size in points.
		Objects with external contours that are smaller than the limit are not identified as blobs.
		@param[in] minContourSize - the minimal contour size in points.
		@return PXC_STATUS_NO_ERROR - successful operation.
		@return PXC_STATUS_PARAM_UNSUPPORTED - invalid minContourSize value (in this case, the last valid value will be retained).
		@see QueryMinContourSize
	*/
	virtual pxcStatus PXCAPI SetMinContourSize(pxcI32 minContourSize) = 0;

	/** 
		@brief Get the minimal contour size in points.
		@return The minimal contour size in points.
		@see SetMinContourSize
	*/
	virtual pxcI32 PXCAPI QueryMinContourSize(void) const = 0;
};
 