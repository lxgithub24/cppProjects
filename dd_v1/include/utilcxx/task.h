 
 
/**
 * @file task.h
 * @author lishilong(com@baidu.com)
 * @date 2013/08/07 09:58:07
 * @brief 
 *  
 **/




#ifndef  __SPEECH_UTIL_TASK_H_
#define  __SPEECH_UTIL_TASK_H_
#include<pthread.h>

namespace speech {
namespace util {

class Task {
public:
    explicit Task(int channel):channel_(channel){
    }

    virtual ~Task(){
    }

    virtual void handle(){};

    void runHandle() {
       pthread_testcancel();
       handle();
       pthread_testcancel();
    }

    inline int channel() {
       return channel_;
    }
protected:
    int channel_;
};

}
}















#endif  //__TASK_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
