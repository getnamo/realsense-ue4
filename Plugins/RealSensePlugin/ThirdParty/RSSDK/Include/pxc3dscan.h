/*******************************************************************************
INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2014 Intel Corporation. All Rights Reserved.
*******************************************************************************/
/// @file pxc3dscan.h
/// 3D Capture video module interface

#ifndef PXC3DSCAN_H
#define PXC3DSCAN_H
#include "pxccapture.h"

class PXC3DScan : public PXCBase 
{
public:
    /// PXC3DScan modules operating modes
    /// The operating mode is changed by calling SetMode. 
    /// The default operating mode is TARGETING.
    /// While the operating mode is set to TARGETING, the scanning 
    /// volume is cleared and set to a space in front of the camera.
    /// When the operating mode is set to SCANNING, the scanning
    /// volume is locked to world space and each input frame is 
    /// projected and accumulated into the scanning volume.
    enum Mode { TARGETING, SCANNING };
    /// Query the current PXC3DScan Mode
    virtual Mode PXCAPI QueryMode(void)=0;
    /// Set the PXC3DScan Mode
    virtual pxcStatus PXCAPI SetMode(Mode in_mode)=0;

    /// PXC3DScan preview image access
    /// The preview image is a rendered approximation of the scanning volume
    /// from the perspective of the camera. A different image is available 
    /// each time a frame is processed.
    /// When the operating mode is set to TARGETING, the preview shows objects
    /// within the extent of the scanning volume.
    /// When the operating mode is set to SCANNING, the preview shows the 
    /// accumulated scanning volume data.
    virtual PXCImage* PXCAPI AcquirePreviewImage(void)=0;

    /// PXC3DScan mesh formats supported by Reconstruct
    enum FileFormat { OBJ, PLY, STL };
    /// PXC3DScan generation of file in standard mesh format from the scanning volume.
    virtual pxcStatus PXCAPI Reconstruct(FileFormat     in_type, 
                                         const pxcCHAR* in_filename)=0;
    /// PXC3DScan utility to convert FileFormat value to a string
    __inline static const pxcCHAR * FileFormatToString(FileFormat format) {
        switch (format) {
        case OBJ:        return (const pxcCHAR*)L"obj";
        case PLY:        return (const pxcCHAR*)L"ply";
        case STL:        return (const pxcCHAR*)L"stl";
        }
        return (const pxcCHAR*)L"";
    }

    PXC_CUID_OVERWRITE(PXC_UID('S','C','I','1'));
};
#endif


