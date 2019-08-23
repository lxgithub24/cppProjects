 
/**
 * @file downsampling.h
 * @author lishilong(com@baidu.com)
 * @date 2014/02/21 11:43:08
 * @brief 
 *  
 **/




#ifndef  __DOWNSAMPLING_H_
#define  __DOWNSAMPLING_H_
#include<stdlib.h>
#include<string.h>
#include<iostream>
int down_sampling(short *input_audio_16k,short * output_audio_8k,size_t input_audio_len,size_t &output_audio_len);




#endif  //__DOWNSAMPLING_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
