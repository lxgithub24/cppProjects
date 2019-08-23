 /**
  * @file MFE.h
  * @author songhui(com@xiaojukeji.com)
  * @date 2016/03/12 15:46:50
  * @brief 
  **/

#ifndef  __MFE_H_
#define  __MFE_H_

/** @brief Edition Number */
#define     EDITION     601    // multi-instance

/** @brief Pre-Definition */
#define     MFE_SUCCESS                 0
#define     MFE_ERROR_UNKNOWN           -100
#define     MFE_STATE_ERR               -102
#define     MFE_POINTER_ERR             -103
#define     MFE_MEMALLOC_ERR            -107
#define     MFE_TIMEOUT_ERR             -108
#define     MFE_PARAMRANGE_ERR          -109
#define     MFE_SEND_TOOMORE_DATA_ONCE  -118
#define     MFE_VAD_INIT_ERROR          -120

#define     PARAM_MAX_WAIT_DURATION     1  //最大等待时间  60，大概一秒多点的时间
#define     PARAM_MAX_SP_DURATION       2  //最大语音长度  1875，最长说话时间60秒
#define     PARAM_MAX_SP_PAUSE          3  //语音停顿间隔，默认30
#define     PARAM_MIN_SP_DURATION       4  //最小语音长度，默认4
#define     PARAM_SLEEP_TIMEOUT         5
#define     PARAM_ENERGY_THRESHOLD_SP   6
#define     PARAM_ENERGY_THRESHOLD_EP   7
#define     PARAM_OFFSET                8
#define     PARAM_SPEECH_END            9  //端点检测超时，尾部静音长度，结束
#define     PARAM_SPEECH_MODE           10 //识别模式， 0：（搜索模式）表示的是单句输入，1：（输入法模式）表示多句输入
#define		PARAM_NEED_VAD              11      //是否开启VAD的功能
#define     PARAM_NEED_COMPRESS         12                	 // «∑Ò–Ë“™—πÀı
#define     PARAM_SAMPLE_RATE           13                   //…Ë÷√≤…—˘¬ 
#define     PARAM_CODE_FORMAT           14                   //…Ë÷√—πÀı∏Ò Ω
#define     PARAM_SPEECHIN_THRESHOLD_BIAS   15  //语音进入阈值，数值越大要求的声音质量越高
#define     PARAM_SPEECHOUT_THRESHOLD_BIAS  16  //语音退出阈值，数值越大要求的声音质量越高
#define     PARAM_BITRATE_NB            17
#define     PARAM_BITRATE_WB            18
#define     PARAM_BITRATE_OPUS_8K       19
#define     PARAM_BITRATE_OPUS_16K      20
#define     PARAM_MODE_CMB              21
#define     PARAM_CARLIFE_ENABLE        22

#define     PARAM_VADTHR_SILENCE_SP         0
#define     PARAM_VADTHR_SLIGHTNOISE_SP     1
#define     PARAM_VADTHR_NOISE_SP           2
#define     PARAM_VADTHR_SILENCE_EP         3
#define     PARAM_VADTHR_SLIGHTNOISE_EP     4
#define     PARAM_VADTHR_NOISE_EP           5

#define     MFE_FORMAT_BV32_8K          0
#define     MFE_FORMAT_PCM_8K           1
#define     MFE_FORMAT_ADPCM_8K         2
#define     MFE_FORMAT_AMR_8K           3
#define     MFE_FORMAT_BV32_16K         4
#define     MFE_FORMAT_PCM_16K          5
#define     MFE_FORMAT_AMR_16K          7
#define     MFE_FORMAT_FEA_16K_2_3_30   20
#define     MFE_FORMAT_FEA_16K_Shake    21
#define     MFE_FORMAT_OPUS_8K          64
#define     MFE_FORMAT_OPUS_16K         68

#define     VAD_FRAMELENGTH             256

#define     THREAD_SLEEPTIME            2000

#define     DYNAMIC_THR_ADJUST
#define     USING_BV32
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#include    "MFE.h"
#include    "PreDefinition.h"
#include    "BaseLib.h"
#include    <stddef.h>
#include    <math.h>
#include    <pthread.h>
#include    <unistd.h>
#ifdef  USING_BV32
/** @brief Include for BV32 */
#include    "./bvcommon/typedef.h"
#include    "./bvcommon/bvcommon.h"
#include    "./bv32/bv32cnst.h"
#include    "./bv32/bv32strct.h"
#include    "./bv32/bv32.h"
#include    "./bvcommon/utility.h"
#include    "./bv32/bitpack.h"
#include    "./bvcommon/memutil.h"
#endif

#include    "drc.h"

class CMFE
{
    public:

    /** @brief MFE Parameters */
    enum MFEState
    {
        UNKNOWN     =   0,
        BOOT        =   1,
        IDLE        =   2,
        RUN         =   3,
    };

    enum VADState
    {
        SILENCE             =   0,
        SILENCE_TO_SPEECH   =   1,
        SPEECH_TO_SILENCE   =   2,
        NO_SPEECH           =   3,
        SPEECH_TOO_SHORT    =   4,
        SPEECH_TOO_LONG     =   5,
        SPEECH_PAUSE        =   6,  /**< @brief For Speech Input */
        NO_DETECT           =   7,  /**< @brief No Data Arrived */
    };

    enum DetectResult
    {
        RET_SILENCE             =   0,
        RET_SILENCE_TO_SPEECH   =   1,
        RET_SPEECH_TO_SILENCE   =   2,
        RET_NO_SPEECH           =   3,
        RET_SPEECH_TOO_SHORT    =   4,
        RET_SPEECH_TOO_LONG     =   5,
        RET_RESERVE1            =   6,
        RET_RESERVE2            =   7,
        RET_RESERVE3            =   8,
        RET_RESERVE4            =   9,
    };

    public:

/** @brief Global Variable Definition */
/** @brief Global Memory Data Space: 60KB in all.(30 * nFrameLength(256))
 * Line1: pSubbandDivision,     Length: nSubbandNum + 1     (DWORD)
 * Line2: pProbability,         Length: nFrameLength
 * Line3: pPowSpec,             Length: nFrameLength 
 * Line4: pSubbandEntropy,      Length: nSubbandNum
 * Line5: pTempEntropyArray,    Length: nL
 * Line6 ~ 6+nL-1: ppSubbandData,   Length: nL(17) * nSubbandNum
 * Line23: Default
 * ... */
double **g_ppMFEDataSpace;          /**< @brief Parameters used in VAD processing */
short  *g_pData;                    /**< @brief Original Data */
UWord8 *g_pBVData;                  /**< @brief BV32 Data */
short  *g_pVADResult;               /**< @brief Save VAD Frame-Result */
DWORD SubbandDivisionIdx;
DWORD ProbabilityIdx;
DWORD PowSpecIdx;
DWORD SubbandEntropyIdx;
DWORD TempEntropyArrayIdx;
DWORD SubbandDataIdx;

/** @brief Basic Parameters */
DWORD nCurState;       /**< @brief Current State of LSM */
DWORD nVADCurState;
DWORD nVADLastState;

DDWORD lSample;              /**< @brief Beginning of current segment */
DDWORD lSampleStart;              /**< @brief Beginning of current segment */
DDWORD lSampleEnd;                /**< @brief End of current segment */
DDWORD lBVStartLoc;               /**< @brief Start Loc of VAD Cache g_pBVData */
DDWORD lBVCurLoc;                 /**< @brief Current Loc of VAD Cache */
DDWORD lVADResultStartLoc;        /**< @brief VAD Result Start Loc (Frame) */
DDWORD lVADResultCurLoc;          /**< @brief VAD Result Current Loc (Frame) */
DDWORD lFrameCnt;                 /**< @brief Frame Counter (Last time) */
DDWORD lFrameCntTotal;            /**< @brief For SpeechMode = 1, Total Frame Cnt */
DWORD nStartFrame;
DWORD nEndFrame;
double dThr_InSpeech;
double dThr_OutSpeech;
DWORD nIsFinishFlag;              /**< @brief User Finish Voluntarily or Not */

/** @brief Basic Constant */
DWORD nSampleRate;
DWORD nCodeFormat;
DWORD nFrameLength;
DWORD nFrameShift;
DWORD nFFTOrder;
bool bNoiseDetectionFlag;         /**< @brief Noise Detection Flag, Default: false */
int iLogLevel;

/** @brief VAD Parameters */
DWORD nN;
DWORD nL;
DWORD nSubbandNum;
double dQ;
double dBeta;
double dTheta;
double dLambda;
DWORD nStartBackFrame;
/** @brief For Speed */
DWORD nH;
double dLambda_hat;

/** @brief 10 Open and Adjustable Parameters */
DWORD nMax_Wait_Duration_Init;
DWORD nMax_Speech_Duration_Init;
DWORD nMax_Speech_Pause_Init;
DWORD nMin_Speech_Duration_Init;
DWORD nSleep_Timeout_Init;     /**< @brief Max Speech Length: 18s */
double dThreshold_Start_Init;
double dThreshold_End_Init;
DWORD nOffset_Init;             /**< @brief Frames */
DWORD nSpeech_End_Init;
DWORD nSpeech_Mode_Init;        /**< @brief 1 means Continuous Speech Detect */
double dThrBias_SpeechIn_Init;
double dThrBias_SpeechOut_Init;

/** @brief Adjustable Parameters for Voice Input Method */
double dThrBias_SpeechIn_BI_Silence_Init;
double dThrBias_SpeechIn_BI_Slightnoise_Init;
double dThrBias_SpeechIn_BI_Noise_Init;
double dThrBias_SpeechOut_BI_Silence_Init;
double dThrBias_SpeechOut_BI_Slightnoise_Init;
double dThrBias_SpeechOut_BI_Noise_Init;

DWORD nMax_Wait_Duration;
DWORD nMax_Speech_Duration;
DWORD nMax_Speech_Pause;
DWORD nMin_Speech_Duration;
DWORD nSleep_Timeout;
double dThreshold_Start;
double dThreshold_End;
DWORD nOffset;
DWORD nSpeech_End;
DWORD nSpeech_Mode;
double dThrBias_SpeechIn;
double dThrBias_SpeechOut;

double dThrBias_SpeechIn_BI_Silence;
double dThrBias_SpeechIn_BI_Slightnoise;
double dThrBias_SpeechIn_BI_Noise;
double dThrBias_SpeechOut_BI_Silence;
double dThrBias_SpeechOut_BI_Slightnoise;
double dThrBias_SpeechOut_BI_Noise;

short nIn_Speech_Threshold;
DWORD nOffsetLength;
DWORD nVADInnerCnt;    /**< @brief If there is eight "1", change VADState */
DWORD nVADInnerZeroCnt;
DWORD nSpeechEndCnt;    /**< @brief Used ONLY in SpeechMode 1 */
DWORD nSpeechEncLength;    /**< @brief Speech Encoder Framelength: BV: 80; AMR: 320 */

/** @brief For Endpoint Data Cache */
DWORD nFindPossibleEndPoint;
DWORD nPossible_Speech_Pause_Init;
DWORD nPossible_Speech_Pause;
DWORD nPossible_Speech_Start;

/** @brief For More Accurate Startpoint & Endpoint Detection */
double dMaxSubEntro;   /**< @brief Max Subband Entropy during ONE SPEECH */
double dMinSubEntro;    /**< @brief Max Subband Entropy during ONE SPEECH */
double dThrAdjust_SpeechOut;      /**< @brief Endpoint Threshold adjusted dynamicly */
double dThrAdjust_SpeechIn;      /**< @brief Startpoint Threshold adjusted dynamicly */
DWORD  nAmp;
double pThrSubEntro[8];
double dBasicEnergy;
double pStartEnergy[8];
double pZeroPass[8];
double pMean[4];
DWORD  nBackEnd;        /**< @brief Backwards Frame in Speech pause (Input Mode) */

/** @brief For Keypad Tone Removing or Filtering */
bool bKeypadFiltering;    /**< @brief Keypad Tone Filtering or Not */
DWORD nKeyToneRange;      /**< @brief The First 500ms */
DWORD nKeyToneStep;
DWORD nKeyToneOffset;       /**< @brief Offset 50ms */
DWORD nKeyTonePeakThr;
DWORD nKeyToneLeftThr;
DWORD nKeyToneRightThr;
DWORD nKeyToneLeftRange;      /**< @brief 300ms silence on the left side of the Peak */
DWORD nKeyToneRightRange;
DWORD nKeyToneDelta;

/** @brief For First 200ms Modification */
bool bInvalidRecModification;
DWORD nModificationRange;         /**< @brief The First 500ms */
DWORD nThr_ZeroPass;
DWORD nThr_Energy;

/** @brief new para*/
bool nVAD;
bool nCompress;
bool isFirst;
bool isSilence;

/** @brief Mode Combination */
DWORD nModeComb;

/** @brief Carlife Noisy Environment endpoint detection 201450728 */
DWORD nCarlife;
DRC_HANDLE drc_handle;
/** @brief Carlife Noisy Environment endpoint detection End 201450728 */

/** @brief BV32 Variables and Parameters */
int     frame;
int     sizebitstream, sizestate;
int     frsz;
struct  BV32_Encoder_State* state;
struct  BV32_Bit_Stream* bs;
UWord8  PackedStream[20];

/** @brief MultiThread Mutex */
pthread_mutex_t MyMutex;
pthread_cond_t MyCond;
struct timespec ts;
    
    double VAD_SubbandEntropy_CalThreshold();
    bool VAD_SubbandEntropy_CalSubEntro(short *pData);
    short VAD_SubbandEntropy_Process(short *pData);

    public:

    CMFE();
    int mfeInit();
    int mfeExit();
    int mfeReset();    // songhui 20180521
    int mfeSendData(short* pDataIn, int iLen);
    int mfeGetCallbackData(char* pDataOut, int iLen);
    int mfeDetect();
    int mfeSetParam(int type, int val);
    int mfeGetParam(int type);
    int mfeEnableNoiseDetection(bool val);
    void mfeSetLogLevel(int iLevel);
    int mfeSetVADParam(int type, int val);  //  Extended Set Param Func, for Shanghai Voice Input Method
    int mfeGetStartLocation();
 };

#endif  //__MFE_H_
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
