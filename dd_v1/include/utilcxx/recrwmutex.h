 
 
/**
 * @file recrwmutex.h
 * @author lishilong(com@baidu.com)
 * @date 2013/09/17 15:43:22
 * @brief 
 *  
 **/




#ifndef  __SPEECH_UTIL_RECRWMUTEX_H_
#define  __SPEECH_UTIL_RECRWMUTEX_H_
#include<pthread.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<unistd.h>
#include<signal.h>

namespace speech {
namespace util {
class RecRWMutex {
public:
    RecRWMutex() {
        init();
    }

    ~RecRWMutex() {
        destroy();
    }

    inline void rlock() {
        int ret = pthread_rwlock_rdlock(&rec_rwmutex_);
        if (ret !=0 ) {
        
        }
    }

    inline void wlock() {
        int ret = pthread_rwlock_wrlock(&rec_rwmutex_);
        if (ret !=0 ) {
        
        }
    }

    inline void unlock() {
       int ret = pthread_rwlock_unlock(&rec_rwmutex_);
       if (ret != 0) {
       
       }
    }
private:
//    RecRWMutex(const RecRWMutex&);
//    RecRWMutex & operator = (const RecRWMutex);

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


    inline void init() {

        int ret = pthread_rwlock_init(&rec_rwmutex_,NULL);
        if (ret != 0)  {
          make_core();
        }
    }

    inline void destroy() {
    
       int ret = pthread_rwlock_destroy(&rec_rwmutex_);
       if (ret !=0) {
          make_core();
       }
    }
   
   pthread_rwlock_t rec_rwmutex_;
};

namespace RecRWMutexLock{
class RLock {
public:
    explicit RLock(RecRWMutex & mutex_):rec_rwmutex_(mutex_) {
       rec_rwmutex_.rlock(); 
    }

    ~RLock() {
      rec_rwmutex_.unlock();
    }
private:
    RLock(const RLock &);
    RLock & operator = (const RLock &);
    RecRWMutex & rec_rwmutex_;
};

class WLock {
public:
    explicit WLock(RecRWMutex & mutex_):rec_rwmutex_(mutex_) {
        rec_rwmutex_.wlock();
    
    }
    ~WLock() {
        rec_rwmutex_.unlock();
    }
private:
    WLock(const WLock &);
    WLock & operator = (const WLock &);
    RecRWMutex & rec_rwmutex_;
};

}

}
}















#endif  //__RECRWMUTEX_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
