/*******************************************************************************
INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2015 Intel Corporation. All Rights Reserved.
*******************************************************************************/
/// @file pxc3dscan.h
/// 3D Capture video module interface
#pragma once
#include "pxccapture.h"

class PXC3DScan : public PXCBase 
{
public:

	PXC_CUID_OVERWRITE(PXC_UID('S', 'C', 'I', '1'));

	/// PXC3DScan mode
    ///
    /// In TARGETING mode, the scanning volume is cleared each 
    /// frame and repositioned to a space in front of the camera.
    ///
    /// When mode is set to SCANNING, the scanning
    /// volume is locked to world space and each input frame is 
    /// projected and accumulated into the scanning volume.
    ///
    /// SetMode(SCANNING) returns PXC_STATUS_ITEM_UNAVAILABLE when 
    /// TARGETING_OPTION_OBJECT_ON_PLANAR_SURFACE_DETECTION
    /// is set and no object is detected.
    ///
    enum Mode { TARGETING, SCANNING };
    /// Returns the PXC3DScan mode
    virtual Mode PXCAPI QueryMode(void) = 0;
    /// Configures the PXC3DScan mode
    virtual pxcStatus PXCAPI SetMode(Mode in_mode) = 0;

    /// PXC3DScan targeting options
    enum TargetingOption { /// Bit-OR'ed values
        NO_TARGETING_OPTIONS = 0,
        OBJECT_ON_PLANAR_SURFACE_DETECTION = 0x0001
    };
    /// Returns the PXC3DScan targeting options
    virtual TargetingOption PXCAPI QueryTargetingOptions(void) = 0;
    /// Configures the PXC3DScan targeting options
    virtual pxcStatus PXCAPI SetTargetingOptions(TargetingOption in_options) = 0;

    /// PXC3DScan preview image
    ///
    /// The preview image is a rendered approximation of the scanning volume
    /// from the perspective of the most recent registered frame. 
    ///
    /// When mode is set to TARGETING, the preview shows objects
    /// within the extent of the scanning volume.
    ///
    /// When mode is set to SCANNING, the preview shows the 
    /// accumulated scanning volume data.
    ///
    virtual PXCImage* PXCAPI AcquirePreviewImage(void) = 0;

    /// PXC3DScan reconstruction file format
    ///
    /// Reconstruct returns an error if called in TARGETING mode
    ///
    enum FileFormat { OBJ, PLY, STL };
    /// PXC3DScan reconstruction file format extension helper
    __inline static const pxcCHAR * FileFormatToString(FileFormat format) {
        switch (format) {
        case OBJ: return (const pxcCHAR*)L"obj";
        case PLY: return (const pxcCHAR*)L"ply";
        case STL: return (const pxcCHAR*)L"stl";
        }
        return (const pxcCHAR*)L"Unknown";
    }

    /// PXC3DScan reconstuction options
    enum ReconstructionOption { /// Bit-OR'ed values
        NO_RECONSTRUCTION_OPTIONS = 0,
        SOLIDIFICATION = 0x0001
    };

    /// PXC3DScan mesh generation
    ///
    /// Reconstruct returns an error if called in TARGETING mode
    ///
    virtual pxcStatus PXCAPI Reconstruct(FileFormat type, const pxcCHAR* filename, const ReconstructionOption options) = 0;
    /// PXC3DScan mesh generation helper function
    __inline pxcStatus Reconstruct(FileFormat in_type, const pxcCHAR* in_filename)
    { 
        return Reconstruct(in_type, in_filename, NO_RECONSTRUCTION_OPTIONS);
    }
};

/// TargetingOption helper function
__inline static PXC3DScan::TargetingOption operator|(PXC3DScan::TargetingOption a, PXC3DScan::TargetingOption b)
{
    return (PXC3DScan::TargetingOption)((int)a | (int)b);
}

__inline static PXC3DScan::TargetingOption operator^(PXC3DScan::TargetingOption a, PXC3DScan::TargetingOption b)
{
	return (PXC3DScan::TargetingOption)((int)a ^ (int)b);
}

/// ReconstructionOption helper function
__inline static PXC3DScan::ReconstructionOption operator|(PXC3DScan::ReconstructionOption a, PXC3DScan::ReconstructionOption b)
{
    return (PXC3DScan::ReconstructionOption)((int)a | (int)b);
}

/// ReconstructionOption helper function
__inline static PXC3DScan::ReconstructionOption operator^(PXC3DScan::ReconstructionOption a, PXC3DScan::ReconstructionOption b)
{
	return (PXC3DScan::ReconstructionOption)((int)a ^ (int)b);
}
