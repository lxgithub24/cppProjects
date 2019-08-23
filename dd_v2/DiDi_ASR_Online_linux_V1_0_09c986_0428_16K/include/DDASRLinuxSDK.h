#ifndef __DDASRLINUXSDK_H__
#define __DDASRLINUXSDK_H__
#include "DDASRMessage.h"
#include "string.h"
#include "pthread.h"
#include "deque"
#include "task_runner.h"
#include "MFE.h"
#define PACK_LEN        3200
namespace dd{
using namespace std;
typedef enum{
	RATE_8K=8,
	RATE_16K=16
}rate_t;
typedef enum{
        ST_INIT,
        ST_RUNNING,
	ST_FETCH,
        ST_FINISH
}status_t;
typedef enum {
	DEBUG = 0,
	NOTICE ,
	TRACE,
	INFO,
	WARNING,
	ERROR,
	FATAL
}Log_t;
typedef enum {
	SUCCESS = 0,
	ERR_MEM,
	ERR_INIT,
	ERR_SESSION,
	ERR_NUM_EXCEED,
	ERR_VAD,
	ERR_SERVER,
	ERR_PARAM,
	ERR_OTHER,
	VAD_END=100,
}err_t;
class DDASRLinuxSDK;
typedef void (*event_callback)(ASRResultM &msg,void*arg) ;
class DDASRLinuxSDK{
public:
	static int _default_instance_count ;//= 10;
	static std::deque<DDASRLinuxSDK*>_sdk;
	static ::speech::util::RecMutex _mutex;
	static string _log_dir;
	static string _log_file;
	static int _log_level ;
	static int _init;
	static int err_no;
	static string get_sdk_version();

public:
	DDASRLinuxSDK(int rate,int vad_on,int bv_on);
	~DDASRLinuxSDK();	
	static int init_env(string log_path,string log_file,int log_level);
	static DDASRLinuxSDK *getInstance(int rate,int vad_on,int bv_on,string &err_msg);
	static void clean_resource(bool force = false);
	void set_event_listener(event_callback cb,void* arg);
	
public:
	int _err_no ;
	string _err_msg;	
	uint64_t _logid;
	int _vad_on;
	int _bv_on;
public:
	status_t status;
	::speech::util::RecMutex _s_mutex;
	::speech::util::RecMutex _task_mutex;
	event_callback _cb;
	::speech::util::AtomicInt concurrency_counter;
public:
	struct timeval  _last_op;
	char _user_arg_buf[512];
	string _sid;
	int _rate;
	int _max_sp_duration;
	int _isLast ;
	size_t _last_pkg_size;
	int _idx;
	string _app_name;
	int _app_pid;
	pthread_mutex_t _lock;
	pthread_cond_t _cond;
	CMFE *_alg ;
	struct timeval _createSessionBegin, _vadBegin,_audioTimeEnd;
	struct timeval _last_audio_push;
public:
	int post(DDASRMessage &msg,string &err_msg);
private:
	int create_session();
	int initVad();
	int sendVad(short * in_data,size_t len, char *vad_data,int &vad_len);
	void update_op();
	uint64_t getTimeCost();
	static void fetch_result(void *arg);
	int  push_audio(boost::shared_array<char> buf,size_t size,string &err_msg);
public:
	void notify();
};
}
#endif 
