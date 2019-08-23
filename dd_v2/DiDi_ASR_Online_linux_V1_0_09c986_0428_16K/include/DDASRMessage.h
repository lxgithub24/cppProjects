#ifndef __DDASRMESSAGE_H__
#define __DDASRMESSAGE_H__
#include "string"
//#include "task_runner.h"
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
namespace dd{
using namespace std;
typedef enum {
	DD_ASR_CMD_CONFIG,
	DD_ASR_CMD_START,
	DD_ASR_CMD_PUSH_AUDIO,
	DD_ASR_CMD_STOP,
	DD_ASR_CMD_CANCEL
}Msg_t;
typedef enum{
	DD_COMMON_PARAM_KEY_DEBUG_LOG_LEVEL,
	DD_COMMON_PARAM_KEY_API_KEY,
	DD_COMMON_PARAM_KEY_ACCECC_KEY,
	DD_ASR_PARAM_KEY_SAMPLE_RATE,
	DD_ASR_PARAM_KEY_APP,
	DD_DATA_CHUNK,
	CALLBACK_ASR_STATUS,
	DD_CALLBACK_ASR_RESULT,
	DDASRStatusStartWorkIng,
        DDASRStatusFlushData,
        DDASRStatusFinish,
        DDASRStatusLongSpeechEnd,
        DDASRStatusError,
        DDASRStatusCancel,
	DD_ASR_SLIENT=100,
	DD_ASR_TOO_LONG=101,
	DD_ASR_PROCESS_BLOCK=102,
	DD_ASR_NO_SPEAK=103,
	DD_ASR_SPEAK_SHORT=104,
	DD_ASR_MIDDLE=105,
	DD_ASR_UNKNOWN=106,
	DD_ASR_FINISH=107,
	DD_ASR_ERR = 108,
}Param_t;
class ASRResultM{
private:
	Param_t _p;
	string _ars_content;
	float _begin;
	float _end;
	float _tc;
	
public:
	void set_parameter(Param_t p);
	void set_asr_content(string asr);
	void get_asr_content(string &asr);
	void set_offset(float begin,float end);
	void set_tc(float tc);
	float get_tc();
	Param_t get_status();
};
class DDASRMessage {
public:
	Msg_t  name;
public:
	string _log_level;
	string _api_key;
	string _accsss_key;
	boost::shared_array<char> _buf;
	size_t _size;
public:
	void set_parameter(Param_t  type,string value);
	void set_parameter(Param_t  type,char *buf,int read_cnt);
	void set_parameter(Param_t  type,int value);
	
};
}
#define MAX_BUF_LIMIT (1024*1024)
#define ASR_TIME_OUT	12	
#endif
