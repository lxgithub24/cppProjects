 
 
/**
 * @file thread.h
 * @author lishilong(com@baidu.com)
 * @date 2013/08/02 14:27:19
 * @brief 
 *  
 **/




#ifndef  __SPEECH_UTIL_THREAD_H_
#define  __SPEECH_UTIL_THREAD_H_

#include<pthread.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<unistd.h>
#include<signal.h>
//#include"com_log.h"
namespace speech{
namespace util{

class BaseThread {
public:
    BaseThread() {
       int ret = pthread_attr_init(&attr_);
       if (ret < 0) {
           make_core();
       }
    }

    virtual ~BaseThread() {
       int ret = pthread_attr_destroy(&attr_);
       if (ret < 0) {
       
       }
    }

    int detach() {
       int ret = pthread_attr_setdetachstate(&attr_,PTHREAD_CREATE_DETACHED);
       if (ret < 0) {
       }

       return ret;
    }

    int start() {
        int ret = pthread_create(&tid_,&attr_,BaseThread::runFunc,this);
        if ( ret < 0 ) {
        }
    
        return ret;
    }

    static void * runFunc(void * ) ;
    
    inline pthread_t get_tid() {
        return tid_;
    }

    virtual int run() = 0;

private:
    static void make_core() {
        struct rlimit rlim,rlim_new;
        if (getrlimit(RLIMIT_CORE, &rlim)==0) {
           rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
           if (setrlimit(RLIMIT_CORE, &rlim_new)!=0) {
              rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
              (void) setrlimit(RLIMIT_CORE, &rlim_new);
           }
        }

        kill(getpid(),SIGABRT);
    }
    pthread_t tid_;
    pthread_attr_t attr_;

};

}
}















#endif  //__THREAD_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
