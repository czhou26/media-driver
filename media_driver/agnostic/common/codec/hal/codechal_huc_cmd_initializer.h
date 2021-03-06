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
//! \file     codechal_huc_cmd_initializer.h
//! \brief    Defines base class for HUC cmd initializer encoder.
//!

#ifndef __CODECHAL_CMD_INITIALIZER_H__
#define __CODECHAL_CMD_INITIALIZER_H__

#ifdef _HEVC_ENCODE_SUPPORTED
#include "codechal_encode_hevc_base.h"
#endif
#include "codechal_encoder_base.h"

#ifdef _VP9_ENCODE_SUPPORTED
class CodechalVdencVp9State;
#endif

// Command initializer command
typedef enum _CODECHAL_CMD_INITIALIZER_CMDTYPE
{
    CODECHAL_CMD1 = 1,
    CODECHAL_CMD2 = 2
} CODECHAL_CMD_INITIALIZER_CMDTYPE;

//! VP9 Params struct
/*
This struct defines VP9 params for huc initializer
*/
#ifdef _VP9_ENCODE_SUPPORTED
struct Vp9CmdInitializerParams
{
    uint8_t                             vdencMvCosts[12] = { 0 };
    uint8_t                             vdencRdMvCosts[12] = { 0 };
    uint8_t                             vdencHmeMvCosts[8] = { 0 };
    uint8_t                             vdencModeCosts[CODEC_VDENC_NUM_MODE_COST] = { 0 };

    uint16_t                            pictureCodingType;

    PCODEC_VP9_ENCODE_SEQUENCE_PARAMS   seqParams;
    PCODEC_VP9_ENCODE_PIC_PARAMS        picParams;
    bool                                segmentationEnabled;
    bool                                segmentMapProvided;
    PCODEC_VP9_ENCODE_SEGMENT_PARAMS    segmentParams;
    bool                                prevFrameSegEnabled;
    uint8_t                             numRefFrames;
    bool                                hmeEnabled;
    uint8_t                             dysRefFrameFlags;
    bool                                dysVdencMultiPassEnabled;
    int                                 currentPass;
    bool                                singleTaskPhaseSupported;
    bool                                lastTaskInPhase;
    bool                                firstTaskInPhase;
    uint32_t                            mode;
    bool                                videoContextUsesNullHw;
    CodechalDebugInterface*             debugInterface;
    bool                                dynamicScalingEnabled;

    // Common
    uint16_t                            sadQpLambda;
    uint16_t                            rdQpLambda;
    bool                                vdencPakOnlyMultipassEnabled;


};
#endif

//! Command Initializer class
/*
This class defines huc command initializer
*/
class CodechalCmdInitializer
{
public:
    bool                                        m_pakOnlyPass;
    bool                                        m_acqpEnabled;
    bool                                        m_brcEnabled;
    bool                                        m_streamInEnabled;
    bool                                        m_roundingEnabled;
    bool                                        m_panicEnabled;
    bool                                        m_roiStreamInEnabled;
    bool                                        m_tileReplayEnabled;
    int32_t                                     m_currentPass;

    CodechalEncoderState                        *m_encoder = nullptr;                //!< Pointer to ENCODER base class
    MOS_INTERFACE                               *m_osInterface = nullptr;            //!< OS interface
    MhwMiInterface                              *m_miInterface = nullptr;            //!< Common Mi Interface
    MhwVdboxVdencInterface                      *m_vdencInterface = nullptr;
    void*                                       m_picParams = nullptr;               //!< Pointer to picture parameter
    void*                                       m_seqParams = nullptr;               //!< Pointer to sequence parameter
    void*                                       m_sliceParams = nullptr;             //!< Pointer to slice parameter

#ifdef _VP9_ENCODE_SUPPORTED
    //VP9 related changes
    Vp9CmdInitializerParams                     m_vp9Params;
#endif
    CodechalHwInterface*                        m_hwInterface;
    MHW_BATCH_BUFFER                            m_cmdInitializer2ndLevelBatchBuffer[3];
    MOS_RESOURCE                                m_cmdInitializerDmemBuffer[3];
    MOS_RESOURCE                                m_cmdInitializerDataBuffer[3];

    MHW_BATCH_BUFFER                            m_cmdInitializerDysScaling2ndLevelBatchBuffer;
    MOS_RESOURCE                                m_cmdInitializerDysScalingDmemBuffer;
    MOS_RESOURCE                                m_cmdInitializerDysScalingDataBuffer;

    static constexpr uint32_t CODECHAL_CMD1_SIZE = 120;
    static constexpr uint32_t CODECHAL_CMD2_SIZE = 148;

public:
    //!
    //! \brief    Constructor
    //!

    CodechalCmdInitializer(CodechalEncoderState *encoder);

    //!
    //! \brief   Default Constructor
    //!
    CodechalCmdInitializer() {};
    //!

    //!
    //! \brief    Destructor
    //!
    virtual ~CodechalCmdInitializer() {};

    //!
    //! \brief    Set all the data of the InputCom of command initializer HuC FW
    //!
    //! \return   MOS_STATUS
    //!           MOS_STATUS_SUCCESS if success, else fail reason
    //!
    MOS_STATUS CmdInitializerSetConstData(
        PMOS_INTERFACE                              osInterface,
        MhwMiInterface                              *miInterface,
        MhwVdboxVdencInterface                      *vdencInterface,
        void*                                       seqParams,
        void*                                       picParams,
        void*                                       sliceParams,
        bool                                        pakOnlyPass,
        bool                                        acqpEnabled,
        bool                                        brcEnabled,
        bool                                        streaminEnabled,
        bool                                        roiStreamInEnabled,
        bool                                        roundingEnabled,
        bool                                        panicEnabled,
        bool                                        tileReplayEnabled,
        int32_t                                     currentPass
        );

    //!
    //! \brief    Set DMEM of command initializer HuC FW
    //!
    //! \param    [in] brcEnabled
    //!           Indicate if brc is enabled
    //!
    //! \return   MOS_STATUS
    //!           MOS_STATUS_SUCCESS if success, else fail reason
    //!
    MOS_STATUS CmdInitializerSetDmem(bool brcEnabled);

    //!
    //! \brief    Executes command initializer HuC FW
    //!
    //! \param    [in] brcEnabled
    //!           Indicate if brc is enabled
    //! \param    [in] secondlevelBB
    //!           Second level batch buffer
    //!
    //! \return   MOS_STATUS
    //!           MOS_STATUS_SUCCESS if success, else fail reason
    //!
    MOS_STATUS CmdInitializerExecute(bool brcEnabled, PMOS_RESOURCE secondlevelBB);

    //!
    //! \brief    Set Add Commands to BatchBuffer
    //!
    //! \param    [in] commandtype
    //!           Command type, type 1 or type 2 command
    //!           [in] cmdBuffer
    //!           Command buffer
    //!           [in] addToBatchBufferHuCBRC
    //!           Flag to mention if the scenario is BRC or CQP
    //!
    //! \return   MOS_STATUS
    //!           MOS_STATUS_SUCCESS if success, else fail reason
    //!
    virtual MOS_STATUS SetAddCommands(uint32_t commandtype, PMOS_COMMAND_BUFFER cmdBuffer, bool addToBatchBufferHuCBRC)
    {
        MOS_STATUS eStatus = MOS_STATUS_SUCCESS;
        return eStatus;
    }
#ifdef _VP9_ENCODE_SUPPORTED
    //!
    //! \brief    Free Resources
    //!
    void CmdInitializerVp9FreeResources();

    //!
    //! \brief    Allocate resources for VP9
    //!
    //! \return   MOS_STATUS
    //!           MOS_STATUS_SUCCESS if success, else fail reason
    //!
    MOS_STATUS CmdInitializerVp9AllocateResources(CodechalVdencVp9State *state);


    //!
    //! \brief    Set all const VP9 data of the InputCom of command initializer HuC FW
    //!
    //! \param    [in] state
    //!           Pointer to CodechalVdencVp9State
    //!
    //! \return   MOS_STATUS
    //!           MOS_STATUS_SUCCESS if success, else fail reason
    //!
    MOS_STATUS CommandInitializerSetVp9Params(CodechalVdencVp9State *state);

    //!
    //! \brief    Set DMEM of command initializer HuC FW for VP9
    //! \return   MOS_STATUS
    //!           MOS_STATUS_SUCCESS if success, else fail reason
    //!
    MOS_STATUS CmdInitializerVp9SetDmem();

    //!
    //! \brief    Executes VP9 command initializer HuC FW
    //!
    //! \param    [in] cmdBuffer
    //!           Command buffer
    //! \param    [in] picStateBuffer
    //!           Picture state buffer
    //!
    //! \return   MOS_STATUS
    //!           MOS_STATUS_SUCCESS if success, else fail reason
    //!
    MOS_STATUS CmdInitializerVp9Execute(PMOS_COMMAND_BUFFER cmdBuffer, PMOS_RESOURCE picStateBuffer);

    //!
    //! \brief    Set Add Commands to BatchBuffer for VP9
    //!
    //! \param    [in] commandtype
    //!           Command type, type 1 or type 2 command
    //!           [in] cmdBuffer
    //!           Command buffer
    //!
    //! \return   MOS_STATUS
    //!           MOS_STATUS_SUCCESS if success, else fail reason
    //!
    virtual MOS_STATUS AddCommandsVp9(uint32_t commandtype, PMOS_COMMAND_BUFFER cmdBuffer) 
    { 
        return MOS_STATUS_SUCCESS; 
    }
#endif
};
using PCODECHAL_CMD_INITIALIZER = class CodechalCmdInitializer*;

#endif  //__CODECHAL_CMD_INITIALIZER_H__
