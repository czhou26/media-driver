/*
* Copyright (c) 2007-2017, Intel Corporation
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/
//!
//! \file      cm_wrapper_os.h  
//! \brief     Contains CM Device creation/destory definitionsthe and function tables for CM Ults  
//!

#ifndef __CM_WRAPPER_OS_H__
#define __CM_WRAPPER_OS_H__

#include "cm_func.h"
#include "cm_device.h"
#include "cm_def.h"

#define CM_OSAL_SURFACE_FORMAT uint32_t
#define CM_SURFACE_FORMAT_R8U  62
#define CM_SURFACE_FORMAT_R16U 57


//////////////////////////////////////////////////////////////////////////////////////
// Thin CMRT definition -- START
//////////////////////////////////////////////////////////////////////////////////////

typedef struct _CM_CREATECMDEVICE_PARAM
{
    uint32_t                    DevCreateOption;            // [in]  Dev create option
    pCallBackReleaseVaSurface   pCallBackReleaseVaSurf;	    // [in]  Function Pointer to free Libva surface
    void                        *pCmDeviceHandle;           // [out] pointer to handle in driver
    uint32_t                    iVersion;                   // [out] the Cm version
    uint32_t                    iDriverStoreEnabled;        // [out] DriverStoreEnable flag
    int32_t                     iReturnValue;               // [out] the return value from CMRT@UMD
}CM_CREATECMDEVICE_PARAM, *PCM_CREATECMDEVICE_PARAM;

typedef struct _CM_CREATESURFACE2D_PARAM
{
    uint32_t    iWidth;                     // [in] width of 2D texture in pixel
    uint32_t    iHeight;                    // [in] height of 2D texture in pixel
    CM_OSAL_SURFACE_FORMAT Format;          // [in] 2D texture format in OS layer.

    union
    {
        uint32_t    index2DinLookupTable;   // [in] surface 2d's index in look up table. Not used on Linux.
        uint32_t    uiVASurfaceID;          // [in] libva-surface 2d's index in media driver
    };

    void        *vaSurface;                   // [in] Pointer to Libva Surface
    void        *pCmSurface2DHandle;         // [out] pointer of CmSurface2D used in driver
    bool        bIsCmCreated;               // [in] Is the 2D surface created by CM?
    int32_t     iReturnValue;               // [out] the return value from driver

    bool        bIsLibvaCreated;            // [in] if the surface created via libva
    void        *pVaDpy;                     // [in] VaDisplay used to free va sruface
}CM_CREATESURFACE2D_PARAM, *PCM_CREATESURFACE2D_PARAM;

int32_t CmFillMosResource(
    VASurfaceID        iVASurfaceID, 
    VADriverContext*   pUMDCtx,
    PMOS_RESOURCE      pOsResource);

MOS_FORMAT              CmOSFmtToMosFmt(CM_OSAL_SURFACE_FORMAT format);
CM_OSAL_SURFACE_FORMAT  CmMosFmtToOSFmt(MOS_FORMAT format);

int32_t CmCreateDevice(VADriverContextP pVaDrvCtx, CmDevice* &pCmDev, uint32_t DevOption);
int32_t CmDestroyDevice(VADriverContextP pVaDrvCtx, CmDevice *pCmDev);

#endif


