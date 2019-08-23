#include "DDASRLinuxSDK.h"
#include "DDASRMessage.h"
#include "string"
#include "stdio.h"
using namespace dd;
using namespace std;
#define MAX_WAV_SIZE 1024*1024*1024
int read_mch_wav( const char *mic_file, int *mic_len,char ** mData )
{
    cout << "read_mch_wav mic_file: " << mic_file << "  mic_len：" << mic_len <<  "  mData：" << mData << endl;
    FILE *fp_mic = fopen( mic_file, "rb" );
    if ( fp_mic == NULL )
    {
        printf( "Failed to open mch wav file[%s].\n", mic_file );
        return -1;
    }
    fseek( fp_mic, 0, SEEK_END );
    *mic_len = ftell( fp_mic );
    if ( *mic_len > MAX_WAV_SIZE )
    {
        printf( "mch_data overflows.\n" );
        return -1;
    }
    fseek( fp_mic, 0, SEEK_SET );
    fread( mData[0], sizeof(char), *mic_len, fp_mic );
    fclose( fp_mic );

    return 0;
}
//callbcak定义
void asr_output_callback(ASRResultM& message, void* user_arg){
	string asr ;
	message.get_asr_content(asr);
	//std::cout<<"recv asr content " << asr << "," <<message.get_status()<< endl;
	if( message.get_status() == DD_ASR_MIDDLE){
		//中间结果	
		//std::cout<<"中间结果asr:"<<asr<<","<<string((char*)user_arg)<<endl;
		std::cout << "Middle asr\t"<<message.get_tc()<<"\t"<<string((char*)user_arg)<<"\t"<<asr<<std::endl;
	}
	if( message.get_status() == DD_ASR_FINISH){
		//最后的一个结果	
		std::cout<<"最后结果asr:"<<asr<<","<<string((char*)user_arg)<<endl;
		std::cout << "Final asr\t"<<message.get_tc()<<"\t"<<string((char*)user_arg)<<"\t"<<asr<<std::endl;
	}
	if( message.get_status() == DD_ASR_ERR ){
		//报错
		std::cout<<"未识别asr:"<<asr<<","<<string((char*)user_arg)<<endl;;
	}
}
int test(char*,char*,int);
void m_sleep(int ms){
                struct timeval delay;

                delay.tv_sec =0;

                delay.tv_usec =ms * 1000; // 10 ms

                select(0, NULL,NULL, NULL, &delay);
}
int main(int argc,char **argv){
	//初始化SDK日志环境，只需要配置一次
	dd::DDASRLinuxSDK::init_env("./","log.txt",NOTICE);
	std::cout<<"ver:"<<dd::DDASRLinuxSDK::get_sdk_version()<<endl;
	char * data = new char[MAX_WAV_SIZE];
        int len = 0;
        if( data == NULL ){
                cout << "mem error" <<endl;
                return -1;
        }
        	read_mch_wav(argv[1],&len,&data);
		test(argv[1],data,len);	
	//getchar();
	m_sleep(1000);
	//sleep(5);
//	}

	//sleep(5);
	//getchar();
	//程序结束的时候，清理SDK占有的资源
	dd::DDASRLinuxSDK::clean_resource();
	return 0;
}
int test (char * filename,char *data,int len){
	string err_msg;
	int ret = 0;
	if( len <= 0 || data == NULL ){
		return 0;
	}
	//获取ASR SDK的识别的对象指针，支持 8k／16k
	//vad_on = 0 ：语音长度超过60s时，请设置vad_on = 0
	//vad_on = 1: 语音长度低于60s时，请设置vad_on = 1
	int vad_on = 0;
	int bv_on = 0;
	dd::DDASRLinuxSDK *sdk = DDASRLinuxSDK::getInstance(RATE_16K,vad_on,bv_on,err_msg);
	if(sdk == NULL){
		cout<<"sdk inst "<<filename<<" error:"<<err_msg<<endl;
		return -1;
	}
	int index = 0;
	int round = len/PACK_LEN ;
	//设置识别结果的异步回调函数,当有识别结果时，回调函数会被不断触发
	sdk->set_event_listener(&asr_output_callback, (void*)filename);
	cout<<"sid:"<<sdk->_sid<<",seq:"<<filename<<std::endl;
	cout<<"round:"<< (round)+(len/PACK_LEN==0?0:1) <<endl;
	//传输录音的内容，开始识别语音,当语音过长时，可以多次传输
	for(int i =0; i < round ;i++){
		//usleep(500000);	
		m_sleep(200);
		dd::DDASRMessage push_params;
		push_params.name = dd::DD_ASR_CMD_PUSH_AUDIO;
		push_params.set_parameter(dd::DD_DATA_CHUNK, data+i*PACK_LEN, PACK_LEN); 
		ret = sdk->post(push_params, err_msg);
		// VAD_END 表示自动检测到说话结束，此时会自动结束当前的识别
		// 重新开启一个会话，继续识别后续语音
		if( ret == VAD_END ){
			std::cout<<"detect speech end,continue" <<sdk->_sid<<std::endl;
			test(filename,data+(i+1)*PACK_LEN,len-((i+1)*PACK_LEN));
			return 0;
		}else if( ret !=SUCCESS ){
			std::cout<<" post chunk failed 0 --"<< err_msg<<sdk->_sid<<std::endl;
			//test(filename,data+(i+1)*PACK_LEN,len-((i+1)*PACK_LEN));
			return -1;
		}
	}
	if( len%PACK_LEN != 0 ){
		m_sleep(200);
		dd::DDASRMessage push_params;
                push_params.name = dd::DD_ASR_CMD_PUSH_AUDIO;
                push_params.set_parameter(dd::DD_DATA_CHUNK, data+(len/PACK_LEN)*PACK_LEN, len%PACK_LEN);
		ret = sdk->post(push_params, err_msg);
		if( ret == VAD_END ){
                        std::cout<<"detect speech end" <<sdk->_sid<<std::endl;
                }else	if( ret != SUCCESS ){
                        std::cout<<" post chunk failed 2"<< err_msg<< sdk->_sid<<std::endl;
                        return -1;
                }
	}
	float  _l_len = (len%PACK_LEN/(PACK_LEN*1.0));
	int s_in = _l_len * 200;
	std::cout<<"last2 wait:"<<s_in<<std::endl;
	m_sleep(200);
	//传输录音内容结束,手动结束录音传输
	dd::DDASRMessage push_params;
	push_params.name = DD_ASR_CMD_STOP;//dd::DD_ASR_CMD_PUSH_AUDIO;
	push_params.set_parameter(DD_DATA_CHUNK, NULL, 0);
	ret = sdk->post(push_params, err_msg);
	if( ret !=0 ){
                        std::cout<<" post chunk failed"<< err_msg<<sdk->_sid<<std::endl;
                        return -1;
	}
	return 0;
}

