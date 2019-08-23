
#ifndef __BDSASRCore__asr_log__
#define __BDSASRCore__asr_log__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

namespace speech {
namespace asrcore {
        
#define ASR_LOG_OFF     0
#define ASR_LOG_FATAL   1
#define ASR_LOG_ERROR   2
#define ASR_LOG_WARN    3
#define ASR_LOG_INFO    4
#define ASR_LOG_DEBUG   5
#define ASR_LOG_TRACE   6
        
/**
 * @brief set log level
 *
 */
void set_log_level(int32_t level);

/**
 * @brief check can log or not
 *
 */
bool can_log(int32_t level);
        
/**
 * @brief get file name
 *
 */
const char* get_file_name(const char* full_file_name);
        
#ifdef ASR_CORE_LOG

#ifdef __APPLE__
        
#define LOGLINE(x) LOGLINE_(x)
#define LOGLINE_(x) #x

#define BDS_LOG_TRACE(fmt, arg...) \
do { \
    if (can_log(ASR_LOG_TRACE)){ \
        printf("[TRACE][%s:"LOGLINE(__LINE__)"]"fmt"\n", get_file_name(__FILE__), ##arg); \
    } \
} while (0)
#define BDS_LOG_DEBUG(fmt, arg...) \
do { \
    if (can_log(ASR_LOG_DEBUG)){ \
        printf("[DEBUG][%s:"LOGLINE(__LINE__)"]"fmt"\n", get_file_name(__FILE__), ##arg); \
    } \
} while (0)
#define BDS_LOG_INFO(fmt, arg...)  \
do { \
    if (can_log(ASR_LOG_INFO)){ \
        printf("[INFO][%s:"LOGLINE(__LINE__)"]"fmt"\n", get_file_name(__FILE__), ##arg); \
    } \
} while (0)
#define BDS_LOG_WARNING(fmt, arg...) \
do { \
    if (can_log(ASR_LOG_WARN)){ \
        printf("[WARN][%s:"LOGLINE(__LINE__)"]"fmt"\n", get_file_name(__FILE__), ##arg); \
    } \
} while (0)
#define BDS_LOG_ERROR(fmt, arg...) \
do { \
    if (can_log(ASR_LOG_ERROR)){ \
        printf("[ERROR][%s:"LOGLINE(__LINE__)"]"fmt"\n", get_file_name(__FILE__), ##arg); \
    } \
} while (0)
#define BDS_LOG_FATAL(fmt, arg...) \
do { \
    if (can_log(ASR_LOG_FATAL)){ \
        printf("[FATAL][%s:"LOGLINE(__LINE__)"]"fmt"\n", get_file_name(__FILE__), ##arg); \
    } \
} while (0)
        
#elif  defined __WINDOWS__

#define BDS_LOG_TRACE(fmt, ...) \
do { \
    if (can_log(ASR_LOG_TRACE)){ \
        printf("[TRACE][%s:%d]"fmt"\n", get_file_name(__FILE__), __LINE__, __VA_ARGS__); \
    } \
} while (0)
#define BDS_LOG_DEBUG(fmt, ...) \
do { \
    if (can_log(ASR_LOG_DEBUG)){ \
        printf("[DEBUG][%s:%d]"fmt"\n", get_file_name(__FILE__), __LINE__, __VA_ARGS__); \
    } \
} while (0)
#define BDS_LOG_INFO(fmt, ...)  \
do { \
    if (can_log(ASR_LOG_INFO)){ \
        printf("[INFO][%s:%d]"fmt"\n", get_file_name(__FILE__), __LINE__, __VA_ARGS__); \
    } \
} while (0)
#define BDS_LOG_WARNING(fmt, ...) \
do { \
    if (can_log(ASR_LOG_WARN)){ \
        printf("[WARN][%s:%d]"fmt"\n", get_file_name(__FILE__), __LINE__, __VA_ARGS__); \
    } \
} while (0)
#define BDS_LOG_ERROR(fmt, ...) \
do { \
    if (can_log(ASR_LOG_ERROR)){ \
        printf("[ERROR][%s:%d]"fmt"\n", get_file_name(__FILE__), __LINE__, __VA_ARGS__); \
    } \
} while (0)
#define BDS_LOG_FATAL(fmt, ...) \
do { \
    if (can_log(ASR_LOG_FATAL)){ \
        printf("[FATAL][%s:%d]"fmt"\n", get_file_name(__FILE__), __LINE__, __VA_ARGS__); \
    } \
} while (0)

#else
        
#define LOGLINE(x) LOGLINE_(x)
#define LOGLINE_(x) #x

#define BDS_LOG_TRACE(fmt, arg...) \
do { \
    if (can_log(ASR_LOG_TRACE)){ \
        printf("[TRACE][%s:"LOGLINE(__LINE__)"]"fmt"\n", get_file_name(__FILE__), ##arg); \
    } \
} while (0)
#define BDS_LOG_DEBUG(fmt, arg...) \
do { \
    if (can_log(ASR_LOG_DEBUG)){ \
        printf("[DEBUG][%s:"LOGLINE(__LINE__)"]"fmt"\n", get_file_name(__FILE__), ##arg); \
    } \
} while (0)
#define BDS_LOG_INFO(fmt, arg...)  \
do { \
    if (can_log(ASR_LOG_INFO)){ \
        printf("[INFO][%s:"LOGLINE(__LINE__)"]"fmt"\n", get_file_name(__FILE__), ##arg); \
    } \
} while (0)
#define BDS_LOG_WARNING(fmt, arg...) \
do { \
    if (can_log(ASR_LOG_WARN)){ \
        printf("[WARN][%s:"LOGLINE(__LINE__)"]"fmt"\n", get_file_name(__FILE__), ##arg); \
    } \
} while (0)
#define BDS_LOG_ERROR(fmt, arg...) \
do { \
    if (can_log(ASR_LOG_ERROR)){ \
        printf("[ERROR][%s:"LOGLINE(__LINE__)"]"fmt"\n", get_file_name(__FILE__), ##arg); \
    } \
} while (0)
#define BDS_LOG_FATAL(fmt, arg...) \
do { \
    if (can_log(ASR_LOG_FATAL)){ \
        printf("[FATAL][%s:"LOGLINE(__LINE__)"]"fmt"\n", get_file_name(__FILE__), ##arg); \
    } \
} while (0)
        
#endif  // #ifdef __APPLE__
        
#else // 关闭日志
        
//#define BDS_LOG_TRACE(...) do {} while (0)
//#define BDS_LOG_DEBUG(...) do {} while (0)
//#define BDS_LOG_INFO(...) do {} while (0)
//#define BDS_LOG_WARNING(...) do {} while (0)
//#define BDS_LOG_ERROR(...) do {} while (0)
//#define BDS_LOG_FATAL(...) do {} while (0)

#endif  // #ifdef ASR_CORE_LOG
} // namespace asrcore
} // namespace speech

#include"glog2speech.h"

#define BDS_LOG_TRACE 		NOTICE_LOG
#define BDS_LOG_DEBUG 		DEBUG_LOG
#define BDS_LOG_INFO 		DEBUG_LOG
#define BDS_LOG_WARNING 	WARNING_LOG
#define BDS_LOG_ERROR 		ERROR_LOG
#define BDS_LOG_FATAL 		FATAL_LOG
#endif  /* defined(__BDSASRCore__asr_log__) */
