#include "DDASRLinuxSDK.h"
#include "DDASRMessage.h"
#include "string"
#include "stdio.h"
#include <iostream>
#include <sstream>
# include <stdio.h>
# include <stdlib.h>
using namespace dd;
#define MAX_WAV_SIZE 1024*1024*1024
int read_mch_wav( const char *mic_file, int *mic_len,char ** mData )
{
//    std::cout << "read_mch_wav mic_file: " << mic_file << "  mic_len��" << mic_len <<  "  mData��" << mData << std::endl;
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
//callbcak����
void asr_output_callback(ASRResultM& message, void* user_arg){
	string asr ;
	message.get_asr_content(asr);
	//std::cout<<"recv asr content " << asr << "," <<message.get_status()<< std::endl;
	if( message.get_status() == DD_ASR_MIDDLE){
		//�м���	
		//std::cout<<"�м���asr:"<<asr<<","<<string((char*)user_arg)<<std::endl;
	}
	if( message.get_status() == DD_ASR_FINISH){
		//����һ�����	
		std::cout<<"�����asr:"<<asr<<","<<string((char*)user_arg)<<std::endl;
	}
	if( message.get_status() == DD_ASR_ERR ){
		//����
		//std::cout<<"δʶ��asr:"<<asr<<","<<string((char*)user_arg)<<std::endl;;
	}
}
int test(int,char*,int);
int main(){
	//��ʼ��SDK��־������ֻ��Ҫ����һ��
	dd::DDASRLinuxSDK::init_env("./","log.txt",NOTICE);
//	std::cout<<"ver:"<<dd::DDASRLinuxSDK::get_sdk_version()<<std::endl;
	for(int k = 1; k < 97; k++){
		char * data = new char[MAX_WAV_SIZE];
        int len = 0;
        if( data == NULL ){
            std::cout << "mem error" <<std::endl;
            return -1;
        }
        //���ļ���ȡPCM����
        char num[5];
//        itoa(k, num, 10);
        snprintf(num, sizeof(num), "%d", k);
        string filename = "";
		filename += "test";
		filename += num;
		filename += ".pcm";
        read_mch_wav(filename,&len,&data);
//		for(int j = 30; j >= 0 ; j--){
//			for ( int i= 0 ;i < 1;i++)
//		sleep(5);
		test(0,data,len);
//		}
//		getchar();
		printf( "����asrXXXXXXXXXXXXXXXXX, ��%s��\n", k );
	} 
	//���������ʱ������SDKռ�е���Դ
	dd::DDASRLinuxSDK::clean_resource();
	return 0;
}
int test (int idx,char *data,int len){
	string err_msg;
	int ret = 0;
	if( len <= 0 || data == NULL ){
		return 0;
	}
	//��ȡASR SDK��ʶ��Ķ���ָ�룬֧�� 8k��16k
	dd::DDASRLinuxSDK *sdk = DDASRLinuxSDK::getInstance(RATE_16K,err_msg);
	if(sdk == NULL){
//		std::cout<<"sdk inst "<<idx<<" error:"<<err_msg<<std::endl;
		return -1;
	}
	int index = 0;
	int round = len/PACK_LEN ;
	char thread_seq[20]={0};
//	sprintf("idx:%d",idx);
	//����ʶ�������첽�ص�����,����ʶ����ʱ���ص������ᱻ���ϴ���
	sdk->set_event_listener(&asr_output_callback, (void*)thread_seq);
//	std::cout<<"round:"<< (round)+(len/PACK_LEN==0?0:1) <<std::endl;
	//����¼�������ݣ���ʼʶ������,����������ʱ�����Զ�δ���
	for(int i =0; i < round ;i++){
		dd::DDASRMessage push_params;
		push_params.name = dd::DD_ASR_CMD_PUSH_AUDIO;
		push_params.set_parameter(dd::DD_DATA_CHUNK, data+i*PACK_LEN, PACK_LEN); 
		ret = sdk->post(push_params, err_msg);
		// VAD_END ��ʾ�Զ���⵽˵����������ʱ���Զ�������ǰ��ʶ��
		// ���¿���һ���Ự������ʶ���������
		if( ret == VAD_END ){
//			std::cout<<"detect speech end,continue" <<std::endl;
			sleep(5);
			test(idx+1,data+(i+1)*PACK_LEN,len-((i+1)*PACK_LEN));
			return 0;
		}else if( ret !=SUCCESS ){
//			std::cout<<" post chunk failed"<< err_msg<<std::endl;
			return -1;
		}

	}
	if( len%PACK_LEN != 0 ){
		dd::DDASRMessage push_params;
                push_params.name = dd::DD_ASR_CMD_PUSH_AUDIO;
                push_params.set_parameter(dd::DD_DATA_CHUNK, data+(len/PACK_LEN)*PACK_LEN, len%PACK_LEN);
		ret = sdk->post(push_params, err_msg);
		if( ret == VAD_END ){
//                        std::cout<<"detect speech end" <<std::endl;
        }else if( ret != SUCCESS ){
//         std::cout<<" post chunk failed"<< err_msg<<std::endl;
                return -1;
        }
	} 
	//����¼�����ݽ���,�ֶ�����¼������
	dd::DDASRMessage push_params;
	push_params.name = DD_ASR_CMD_STOP;//dd::DD_ASR_CMD_PUSH_AUDIO;
	push_params.set_parameter(DD_DATA_CHUNK, NULL, 0);
	ret = sdk->post(push_params, err_msg);
	if( ret !=0 ){
//        std::cout<<" post chunk failed"<< err_msg<<std::endl;
        return -1;
	}
	return 0;
}

