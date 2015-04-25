/******************************************************************************
	INTEL CORPORATION PROPRIETARY INFORMATION
	This software is supplied under the terms of a license agreement or nondisclosure
	agreement with Intel Corporation and may not be copied or disclosed except in
	accordance with the terms of that agreement
	Copyright(c) 2011-2015 Intel Corporation. All Rights Reserved.
*******************************************************************************/
#pragma once
#include "pxcimage.h"

/**
	@Class PXCBlobData 
	@brief A class that contains extracted blob and contour line data.
	The extracted data refers to the sensor's frame image at the time PXCBlobModule.CreateOutput() was called.
*/
class PXCBlobData: public PXCBase
{
public:

	/* Constants */
	
	PXC_CUID_OVERWRITE(PXC_UID('B','M','D','T'));
	PXC_DEFINE_CONST(NUMBER_OF_EXTREMITIES,6);
	
	/* Enumerations */

	/** 
	@enum AccessOrderType
	@brief Each AccessOrderType value indicates an order in which the extracted blobs can be accessed.
	Use one of these values when calling QueryBlobByAccessOrder().
	*/
	enum AccessOrderType {
		/// From the nearest to the farthest blob in the scene   
		ACCESS_ORDER_NEAR_TO_FAR = 0
		, ACCESS_ORDER_LARGE_TO_SMALL /// From the largest to the smallest blob in the scene   		
		, ACCESS_ORDER_RIGHT_TO_LEFT /// From the right-most to the left-most blob in the scene   		
	};

	
	/**
		@enum ExtremityType
		@brief The identifier of an extremity point of the extracted blob.
		6 extremity points are identified (see values below).\n
		Use one of the extremity types when calling IBlob.QueryExtremityPoint().
	*/
	enum ExtremityType {
		/// The closest point to the sensor in the tracked blob
		EXTREMITY_CLOSEST=0 
		, EXTREMITY_LEFT_MOST 		/// The left-most point of the tracked blob
		, EXTREMITY_RIGHT_MOST		/// The right-most point of the tracked blob 
		, EXTREMITY_TOP_MOST		/// The top-most point of the tracked blob
		, EXTREMITY_BOTTOM_MOST		/// The bottom-most point of the tracked blob
		, EXTREMITY_CENTER			/// The center point of the tracked blob			
	};

	

	/* Interfaces */

    /** 
		@class IBlob
		@brief An interface that provides access to the blob and contour line data.
    */
    class IBlob
	{
	public:
	
	/**			
			@brief Retrieves the 2D segmentation image of a tracked blob. 	 
			In the segmentation image, each pixel occupied by the blob is white (value of 255) and all other pixels are black (value of 0).
			@param[out] image - the segmentation image of the tracked blob.
			@return PXC_STATUS_NO_ERROR - successful operation.
			@return PXC_STATUS_DATA_UNAVAILABLE - the segmentation image is not available.		
	*/		
	virtual pxcStatus PXCAPI QuerySegmentationImage(PXCImage* & image) const = 0; 
		
	/** 		
		@brief Get the specified extremities point's location.
		@param[in] extremityLabel - the extremity type to be retrieved.
		@param[out] extremityPoint - the extremity point location data.
		@return PXC_STATUS_NO_ERROR - successful operation, or an error code otherwise.
		
		@see ExtremityType
	*/
	virtual PXCPoint3DF32 PXCAPI QueryExtremityPoint(ExtremityType extremityLabel) const = 0; 

	/** 
			@brief Get the number of pixels in the blob.
			@return The number of pixels in the blob.
	*/
	virtual pxcI32 PXCAPI QueryPixelCount() const = 0; 

	
	/** 
			@brief Get the number of contour lines extracted (both external and internal).
			@return The number of contour lines extracted.
	*/
	virtual pxcI32 PXCAPI QueryNumberOfContours() const = 0;
	
	/** 
		@brief Get the point array representing a contour line.
		
		A contour is represented by an array of 2D points, which are the vertices of the contour's polygon.
		
		@param[in] index - the zero-based index of the requested contour line (the maximal value is QueryNumberOfContours()-1).
		@param[in] maxSize - the size of the array allocated for the contour points.
		@param[out] contour - the contour points stored in the user-allocated array.
		
		@return PXC_STATUS_NO_ERROR - successful operation.
		@return PXC_STATUS_PARAM_UNSUPPORTED - the given index or the user-allocated array is invalid.
		@return PXC_STATUS_ITEM_UNAVAILABLE - processImage() is running.
	*/
	virtual pxcStatus PXCAPI QueryContourPoints(const pxcI32 index, const pxcI32 maxSize, PXCPointI32* contour) = 0;

	/** 
		@brief Return true for the blob's outer contour; false for inner contours.
		@param[in] index - the zero-based index of the requested contour.
		@return true for the blob's outer contour; false for inner contours.
	*/
	virtual pxcBool PXCAPI IsContourOuter(const pxcI32 index) const = 0;

	/** 
		@brief Get the contour size (number of points in the contour line).
		This is the size of the points array that you should allocate.
		@param[in] index - the zero-based index of the requested contour line.
		@return The contour size (number of points in the contour line).
	*/
	virtual pxcI32 PXCAPI QueryContourSize(const pxcI32 index) const = 0;	


	};	// class IBlob

public:
	/* General */

	/**
	@brief Updates the extracted blob and contour line data according to the current sensor frame.
	Call this method before calling QueryNumberOfBlobs() and QueryBlobByAccessOrder().
	*/
	virtual pxcStatus PXCAPI Update() = 0;
	
    /* Blob module's Outputs */
	
	/** 
			@brief Get the number of detected blobs.	
			@return The number of detected blobs.
	*/
	virtual pxcI32 PXCAPI QueryNumberOfBlobs(void) const = 0;
		
	/**
		@brief Retrieve an IBlob object using a specific AccessOrder and index (that relates to the given order).
		@param[in] index - the zero-based index of the requested blob (between 0 and QueryNumberOfBlobs()-1 ).
		@param[in] accessOrder - the order in which the blobs are enumerated.
		@param[out] blobData - contains the extracted blob and contour line data.
		
		@return PXC_STATUS_NO_ERROR - successful operation.
		@return PXC_STATUS_PARAM_UNSUPPORTED - index >= configured maxBlobs.
		@return PXC_STATUS_DATA_UNAVAILABLE  - index >= number of detected blobs.  
		
		@see AccessOrderType
	*/
	virtual pxcStatus PXCAPI QueryBlobByAccessOrder(const pxcI32 index, AccessOrderType accessOrderType, IBlob*& blobData) = 0;

};
