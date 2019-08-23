 
 
/**
 * @file recmutex.h
 * @author lishilong(com@baidu.com)
 * @date 2013/08/02 12:40:16
 * @brief 
 *  
 **/




#ifndef  __SPEECH_UTIL_RECMUTEX_H_
#define  __SPEECH_UTIL_RECMUTEX_H_
#include<pthread.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<unistd.h>
#include<signal.h>

namespace speech{
namespace util{

class  RecMutex {
public:
    RecMutex() {

       init();

    }

    ~RecMutex() {

        destroy();
    }

    inline void lock() {
        int ret = pthread_mutex_lock(&rec_mutex_);
        if (ret < 0) {
        
        }
    }

    inline void unlock() {
        int ret = pthread_mutex_unlock(&rec_mutex_);
        if (ret < 0) {
        
        }
    
    }
private:
//    RecMutex(const RecMutex &);
//    RecMutex & operator = (const RecMutex &);

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

    inline void init_mutexattr() {
        int ret = pthread_mutexattr_init(&mattr_);
        if (ret < 0) {
            make_core();
        }

        ret = pthread_mutexattr_settype(&mattr_,PTHREAD_MUTEX_RECURSIVE);
        if (ret < 0) {
            make_core();
        }
    }

    inline void init() {
       init_mutexattr();

       int ret = pthread_mutex_init(&rec_mutex_,&mattr_);
       if (ret < 0) {
           make_core();
       }
    }

    inline void destroy() {
       int ret = pthread_mutexattr_destroy(&mattr_);
       if (ret < 0) {
           make_core();
       }

       ret = pthread_mutex_destroy(&rec_mutex_);
       if (ret < 0) {
          make_core();
       }

    }
    pthread_mutex_t rec_mutex_;
    pthread_mutexattr_t mattr_;

};

namespace RecMutexLock {
class Lock {
public:
    explicit Lock(RecMutex & mutex_):rec_mutex_(mutex_) {
        
        rec_mutex_.lock();

    }

    ~Lock() {

        rec_mutex_.unlock();
    }
private:
    Lock();
    Lock(const Lock &);
    Lock & operator = (const Lock &);

    RecMutex & rec_mutex_;

};
}

}
}















#endif  //__RECMUTEX_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
