/*
* Copyright (c) 2017, Intel Corporation
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
//! \file     codec_def_common.h
//! \brief    Defines common types and macros shared by CodecHal, MHW, and DDI layer
//! \details  This is the base header for all codec_def files. All codec_def may include this file which should not contain any DDI specific code.
//!
#ifndef __CODEC_DEF_COMMON_H__
#define __CODEC_DEF_COMMON_H__

#include "mos_defs.h"
#include "mos_os.h"
#include <math.h>

#define CODEC_MAX_NUM_REF_FRAME             16
#define CODEC_MAX_NUM_REF_FRAME_NON_AVC     4
#define CODEC_NUM_FIELDS_PER_FRAME          2

#define CODEC_NUM_BLOCK_PER_MB              6  //!<  Block number per MB: 4Y + Cb +Cr

// picture coding type
#define I_TYPE          1
#define P_TYPE          2
#define B_TYPE          3
#define NUM_PIC_TYPES   3

#define CODECHAL_MACROBLOCK_HEIGHT          16
#define CODECHAL_MACROBLOCK_WIDTH           16

#define WIDTH_IN_DW(w)  ((w + 0x3) >> 2)

#define ABS(a)          (((a) < 0) ? (-(a)) : (a))
#define SIGNED(code)    (2 * ABS(code) - ((code) > 0))

#define CODEC_SIZE_MFX_STREAMOUT_DATA (16 * sizeof(uint32_t))

/*! \brief Flags for various picture properties.
*/
typedef enum _CODEC_PICTURE_FLAG
{
    PICTURE_TOP_FIELD               = 0x01,
    PICTURE_BOTTOM_FIELD            = 0x02,
    PICTURE_FRAME                   = 0x04,
    PICTURE_INTERLACED_FRAME        = 0x08,
    PICTURE_SHORT_TERM_REFERENCE    = 0x10,
    PICTURE_LONG_TERM_REFERENCE     = 0x20,
    PICTURE_INVALID                 = 0x80,
    PICTURE_RESIZE                  = 0xF0,
    PICTURE_MAX_7BITS               = 0xFF
} CODEC_PICTURE_FLAG;

/*! \brief Information pertaining to a frame's uncompressed surface
*
*   Both to identify and describe the surface.
*/
typedef struct _CODEC_PICTURE
{
    uint8_t                 FrameIdx;   //!< Index for the frame's uncompressed surface
    CODEC_PICTURE_FLAG      PicFlags;   //!< Flags describing picture properties
    uint8_t                 PicEntry;   //!< Unaltered DDI frame information (for debug purposes only)
} CODEC_PICTURE, *PCODEC_PICTURE;

// Forward Declarations
typedef struct _CODEC_REF_LIST CODEC_REF_LIST, *PCODEC_REF_LIST;

// ---------------------------
// Enums
// ---------------------------
typedef enum
{
    TOP_FIELD          = 1,
    BOTTOM_FIELD       = 2,
    FRAME_PICTURE      = 3
} PICTURE_STRUCTURE;

enum REFLIST
{
    LIST_0 = 0,
    LIST_1 = 1
};

/*! \brief Codec standard
*/
enum CODECHAL_STANDARD
{
    // MFX/MFL pipeline
    CODECHAL_MPEG2      = 0,
    CODECHAL_VC1        = 1,
    CODECHAL_AVC        = 2,
    CODECHAL_JPEG       = 3,
    CODECHAL_RESERVED   = 4,    //formerly SVC
    CODECHAL_VP8        = 5,
    CODECHAL_UNDEFINED  = 9,

    // Cenc Decode
    CODECHAL_CENC       = 63,

    // HCP pipeline
    CODECHAL_HCP_BASE   = 64,
    CODECHAL_HEVC       = CODECHAL_HCP_BASE,
    CODECHAL_VP9        = CODECHAL_HCP_BASE + 1,
    CODECHAL_STANDARD_MAX
};

/*! \brief Mode requested (high level combination between CODEC_STANDARD and CODEC_FUCNTION).
*
*   Note: These modes are may be used for performance tagging. Be sure to notify tool owners if changing the definitions.
*/
enum CODECHAL_MODE
{
    CODECHAL_DECODE_MODE_MPEG2IDCT          = 0,
    CODECHAL_DECODE_MODE_MPEG2VLD           = 1,
    CODECHAL_DECODE_MODE_VC1IT              = 2,
    CODECHAL_DECODE_MODE_VC1VLD             = 3,
    CODECHAL_DECODE_MODE_AVCVLD             = 4,
    CODECHAL_DECODE_MODE_JPEG               = 5,
    CODECHAL_DECODE_RESERVED_0              = 6,    // formerly SVC
    CODECHAL_DECODE_MODE_VP8VLD             = 7,
    CODECHAL_DECODE_MODE_HEVCVLD            = 8,
    CODECHAL_DECODE_MODE_HUC                = 9,
    CODECHAL_DECODE_RESERVED_2              = 10,   // formerly AVS
    CODECHAL_DECODE_MODE_MVCVLD             = 11,   // Needed for CP. Not in use by Codec HAL.
    CODECHAL_DECODE_MODE_VP9VLD             = 12,
    CODECHAL_DECODE_MODE_CENC            = 13,   // Only for getting HuC-based DRM command size. Not an actual mode.
    CODECHAL_NUM_DECODE_MODES               = 14,

    CODECHAL_ENCODE_MODE_AVC                = 16,   // Must be a power of 2 to match perf report expectations
    CODECHAL_ENCODE_MODE_MPEG2              = 18,
    CODECHAL_ENCODE_MODE_VP8                = 19,
    CODECHAL_ENCODE_MODE_JPEG               = 20,
    CODECHAL_ENCODE_MODE_HEVC               = 22,
    CODECHAL_ENCODE_MODE_VP9                = 23,
    CODECHAL_NUM_ENCODE_MODES               = 8,

    CODECHAL_NUM_MODES                      = 24,   // Use the value for the last encode mode to determine this
    CODECHAL_UNSUPPORTED_MODE               = 24,
    CODECHAL_MODE_MAX                       = 24
};

// Slice group mask
typedef enum tagSLICE_GROUP_MASK
{
    SLICE_GROUP_START   = 0x1,
    SLICE_GROUP_END     = 0x2,
    SLICE_GROUP_LAST    = 0x4
} SLICE_GROUP_MASK;

typedef struct _CODEC_TRACKED_BUFFER
{
    uint8_t             ucSurfIndex7bits;             // 0xFF means the entry can be re-used
    MOS_SURFACE         sScaled4xSurface;             // Handle of the 4x scaled surfaces
    MOS_SURFACE         sScaled2xSurface;             // Handle of the 2x scaled surfaces
    MOS_SURFACE         sScaled16xSurface;            // Handle of the 16x scaled surfaces
    MOS_SURFACE         sScaled32xSurface;            // Handle of the 32x scaled surfaces
    MOS_SURFACE         sVDEncScaled4xSurface;        // Handle of the 4x scaled ref surfaces generated by VDEnc
    MOS_SURFACE         sVDEncScaled8xSurface;        // Handle of the 8x scaled ref surfaces generated by VDEnc
    MOS_SURFACE         sCopiedSurface;               // Handle of copied surface
    MOS_RESOURCE        resMbCodeBuffer;              // MB Code buffer
    MOS_RESOURCE        resMvDataBuffer;              // MV Data buffer
    bool                bUsedforCurFrame;             // Used for FEI Preenc to mark whether this enty can be reused in multi-call case
} CODEC_TRACKED_BUFFER, *PCODEC_TRACKED_BUFFER;

typedef struct _CODEC_PIC_ID
{
    uint8_t   ucPicIdx;
    uint8_t   ucDMVOffset[CODEC_NUM_FIELDS_PER_FRAME];      // 0 - top field, 1 - bottom field
    bool      bValid;
} CODEC_PIC_ID, *PCODEC_PIC_ID;

#ifdef _AVC_ENCODE_SUPPORTED
struct _CODEC_AVC_REF_PIC_SELECT_LIST;
typedef struct _CODEC_AVC_REF_PIC_SELECT_LIST   *PCODEC_AVC_REF_PIC_SELECT_LIST;
#endif

typedef struct _CODEC_VC1_IC
{
    // Top filed
    uint16_t    wICCScale1;
    uint16_t    wICCShiftL1;
    uint16_t    wICCShiftC1;
    // Bottom field
    uint16_t    wICCScale2;
    uint16_t    wICCShiftL2;
    uint16_t    wICCShiftC2;
} CODEC_VC1_IC, *PCODEC_VC1_IC;

struct _CODEC_REF_LIST
{
    // Shared decoding parameters
    CODEC_PICTURE                       RefPic;
    MOS_RESOURCE                        resRefPic;      // Resource of RefPic
    uint8_t                             ucFrameId;
    int32_t                             iFieldOrderCnt[CODEC_NUM_FIELDS_PER_FRAME];
    uint8_t                             ucDMVIdx[CODEC_NUM_FIELDS_PER_FRAME];
    bool                                bUsedAsRef;
    uint8_t                             ucNumRef;
    uint8_t                             ucAvcPictureCodingType; // used for PAFF case, 0: frame, 1: tff field, 2: invalid, 3: bff field
    CODEC_PICTURE                       RefList[CODEC_MAX_NUM_REF_FRAME];
    int16_t                             sFrameNumber;
    // Cenc Decode: stored 2nd level batch buffer and Pic Param index
    uint8_t                             ucCencBufIdx[2]; // 0 - frame/top field, 1 - bottom field

    // Shared encoding parameters
    uint8_t                             ucMbCodeIdx;
    uint8_t                             ucScalingIdx;
    uint8_t                             ucMADBufferIdx;
    bool                                bMADEnabled;
    bool                                b4xScalingUsed;
    bool                                b16xScalingUsed;
    bool                                b32xScalingUsed;
    bool                                b2xScalingUsed; // 2x scaling currently only used for HEVC CNL. Uses same surface as 32x since 32x is not supported.
    uint8_t                             ucInitialIdx[2][2];
    uint8_t                             ucFinalIdx[2][2];
    uint8_t                             ucQPValue[2];
    MOS_RESOURCE                        resBitstreamBuffer;
    MOS_RESOURCE                        resRefMbCodeBuffer;
    MOS_RESOURCE                        resRefMvDataBuffer;
    MOS_SURFACE                         sRefBuffer;
    MOS_SURFACE                         sRefReconBuffer;
    MOS_SURFACE                         sRefRawBuffer;

    // Codec specific parameters
    union
    {
        // AVC, MVC
        struct
        {
           uint16_t                             usNonExistingFrameFlags;
           bool                                 bUsedAsInterViewRef;
           uint32_t                             uiUsedForReferenceFlags;
#ifdef _AVC_ENCODE_SUPPORTED
           PCODEC_AVC_REF_PIC_SELECT_LIST       pRefPicSelectListEntry;
#endif
           MOS_RESOURCE                         resRefTopFieldMbCodeBuffer;
           MOS_RESOURCE                         resRefBotFieldMbCodeBuffer;
           MOS_RESOURCE                         resRefTopFieldMvDataBuffer;
           MOS_RESOURCE                         resRefBotFieldMvDataBuffer;
        };

        // VC1
        struct
        {
            uint32_t                            dwRefSurfaceFlags;
            CODEC_VC1_IC                        Vc1IcValues[CODEC_NUM_FIELDS_PER_FRAME];
            bool                                bUnequalFieldSurfaceValid;
            uint32_t                            dwUnequalFieldSurfaceIdx;
        };

        // HEVC
        struct
        {
            bool                                bIsIntra;
            bool                                bFormatConversionDone;
        };

        // VP9
        struct
        {
            MOS_SURFACE                         sDysSurface;            // dynamic scaled surface (encoding resolution)
            MOS_SURFACE                         sDys4xScaledSurface;    // dynamic scaled surface (encoding resolution)
            MOS_SURFACE                         sDys16xScaledSurface;   // dynamic scaled surface (encoding resolution)
            uint32_t                            dwFrameWidth;           // in pixel
            uint32_t                            dwFrameHeight;          // in pixel
        };
    };
};

#define CODECHAL_OFFSETOF(TYPE, ELEMENT)                    ((size_t)&(((TYPE *)0)->ELEMENT))
#define CODECHAL_OFFSETOF_IN_DW(TYPE, ELEMENT)              (CODECHAL_OFFSETOF(TYPE, ELEMENT) >> 2)
#define CODECHAL_ADDROF_TYPE(TYPE, ELEMENT, ELEMENT_ADDR)   ((TYPE *)((uint8_t *)(ELEMENT_ADDR) - CODECHAL_OFFSETOF(TYPE, ELEMENT)))

#define CODECHAL_GET_WIDTH_IN_MACROBLOCKS(dwWidth)               (((dwWidth) + (CODECHAL_MACROBLOCK_WIDTH - 1)) / CODECHAL_MACROBLOCK_WIDTH)
#define CODECHAL_GET_HEIGHT_IN_MACROBLOCKS(dwHeight)             (((dwHeight) + (CODECHAL_MACROBLOCK_HEIGHT - 1)) / CODECHAL_MACROBLOCK_HEIGHT)
#define CODECHAL_GET_WIDTH_IN_BLOCKS(dwWidth, dwBlockSize)       (((dwWidth) + (dwBlockSize - 1)) / dwBlockSize)
#define CODECHAL_GET_HEIGHT_IN_BLOCKS(dwHeight, dwBlockSize)     (((dwHeight) + (dwBlockSize - 1)) / dwBlockSize)
#define CODECHAL_GET_4xDS_SIZE_32ALIGNED(dwSize)                 ((((dwSize >> 2) + 31) >> 5) << 5)
#define CODECHAL_GET_2xDS_SIZE_32ALIGNED(dwSize)                 ((((dwSize >> 1) + 31) >> 5) << 5)

// Macros to determin CODEC_PICTURE_FLAG meaning
#define CodecHal_PictureIsTopField(Pic)         (((Pic).PicFlags & PICTURE_TOP_FIELD) != 0)
#define CodecHal_PictureIsBottomField(Pic)      (((Pic).PicFlags & PICTURE_BOTTOM_FIELD) != 0)
#define CodecHal_PictureIsField(pic)            \
    ((CodecHal_PictureIsTopField((pic)) || CodecHal_PictureIsBottomField((pic))) != 0)
#define CodecHal_PictureIsFrame(Pic)            (((Pic).PicFlags & PICTURE_FRAME) != 0)
#define CodecHal_PictureIsInterlacedFrame(Pic)  (((Pic).PicFlags & PICTURE_INTERLACED_FRAME) != 0)
#define CodecHal_PictureIsLongTermRef(Pic)      (((Pic).PicFlags & PICTURE_LONG_TERM_REFERENCE) != 0)
#define CodecHal_PictureIsShortTermRef(Pic)     (((Pic).PicFlags & PICTURE_SHORT_TERM_REFERENCE) != 0)
#define CodecHal_PictureIsInvalid(Pic)          (((Pic).PicFlags & PICTURE_INVALID) != 0)

#define CodecHal_GetFrameFieldHeightInMb(pic, heightInMb, adjustedHeightInMb)     \
{                                                                                   \
    if(CodecHal_PictureIsField((pic)))                                              \
    {                                                                               \
        adjustedHeightInMb = ((heightInMb) + 1) >> 1;                             \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        adjustedHeightInMb = (heightInMb);                                        \
    }                                                                               \
}

#define CodecHalIsDecodeModeVLD(mode)               \
    ((mode == CODECHAL_DECODE_MODE_MPEG2VLD)    ||  \
    (mode == CODECHAL_DECODE_MODE_VC1VLD)       ||  \
    (mode == CODECHAL_DECODE_MODE_AVCVLD)       ||  \
    (mode == CODECHAL_DECODE_MODE_JPEG)         ||  \
    (mode == CODECHAL_DECODE_MODE_VP8VLD)       ||  \
    (mode == CODECHAL_DECODE_MODE_HEVCVLD)      ||  \
    (mode == CODECHAL_DECODE_MODE_VP9VLD))

#define CodecHalIsDecodeModeIT(mode)                \
    ((mode == CODECHAL_DECODE_MODE_MPEG2IDCT)   ||  \
    (mode == CODECHAL_DECODE_MODE_VC1IT))
// ---------------------------
// Functions
// ---------------------------
static __inline uint16_t GetReciprocalScalingValue(uint8_t scaleValue)
{
    uint16_t reciprocalScaleValue;
    if (scaleValue < 2)
    {
        reciprocalScaleValue = 0xffff;
    }
    else
    {
        reciprocalScaleValue = (4096 * 16)/scaleValue;
    }

    return reciprocalScaleValue;
}

// this function covert the input value v to A|B format so v~=B*pow(2, A);
static __inline uint8_t Map44LutValue(uint32_t v, uint8_t max)
{
    uint32_t   maxCost;
    int     D;
    uint8_t   ret;

    if (v == 0)
    {
        return 0;
    }

    maxCost = ((max & 15) << (max >> 4));
    if (v >= maxCost)
    {
        return max;
    }

    D = (int)(log((double)v) / log(2.)) - 3;
    if (D < 0)
    {
        D = 0;
    }

    ret = (uint8_t)((D << 4) + (int)((v + (D == 0 ? 0 : (1 << (D - 1)))) >> D));
    ret = (ret & 0xf) == 0 ? (ret | 8) : ret;

    return ret;
}

static __inline uint32_t CodecHal_GetStandardFromMode(uint32_t mode)
{
    uint32_t standard = CODECHAL_UNDEFINED;

    switch (mode)
    {
    case CODECHAL_DECODE_MODE_MPEG2IDCT:
    case CODECHAL_DECODE_MODE_MPEG2VLD:
    case CODECHAL_ENCODE_MODE_MPEG2:
        standard = CODECHAL_MPEG2;
        break;
    case CODECHAL_DECODE_MODE_VC1IT:
    case CODECHAL_DECODE_MODE_VC1VLD:
        standard = CODECHAL_VC1;
        break;
    case CODECHAL_DECODE_MODE_AVCVLD:
    case CODECHAL_ENCODE_MODE_AVC:
        standard = CODECHAL_AVC;
        break;
    case CODECHAL_DECODE_MODE_JPEG:
    case CODECHAL_ENCODE_MODE_JPEG:
        standard = CODECHAL_JPEG;
        break;
    case CODECHAL_DECODE_MODE_VP8VLD:
    case CODECHAL_ENCODE_MODE_VP8:
        standard = CODECHAL_VP8;
        break;
    case CODECHAL_DECODE_MODE_HEVCVLD:
    case CODECHAL_ENCODE_MODE_HEVC:
        standard = CODECHAL_HEVC;
        break;
    case CODECHAL_ENCODE_MODE_VP9:
    case CODECHAL_DECODE_MODE_VP9VLD:
        standard = CODECHAL_VP9;
        break;
    case CODECHAL_DECODE_MODE_CENC:
        standard = CODECHAL_CENC;
        break;
    default:
        standard = CODECHAL_UNDEFINED;
        break;
    }

    return standard;
}

static __inline int32_t CodecHal_Clip3(int x, int y, int z)
{
    int32_t ret = 0;

    if (z < x)
    {
        ret = x;
    }
    else if (z > y)
    {
        ret = y;
    }
    else
    {
        ret = z;
    }

    return ret;
}
#endif  // __CODEC_DEF_COMMON_H__
