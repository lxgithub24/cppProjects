 
 
/**
 * @file monitor.h
 * @author lishilong(com@baidu.com)
 * @date 2013/08/16 15:46:36
 * @brief 
 *  
 **/




#ifndef  __SPEECH_UTIL_MONITOR_H_
#define  __SPEECH_UTIL_MONITOR_H_
#include<pthread.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<errno.h>

namespace speech {
namespace util {
const int kMillisecond = 1000;//=>s
const int kNanosecond  = 1000000;//=>ms
class Monitor {
public:

    Monitor() {
        init();
    }
    
    ~Monitor() {
        destroy();
    }
    
    int timewait( int m_secs /*Millisecond*/) {
        struct timespec  t_spec;
        clock_gettime(CLOCK_REALTIME, &t_spec);
        t_spec.tv_sec  += m_secs / kMillisecond;
        t_spec.tv_nsec += (m_secs % kMillisecond) * kNanosecond;

        pthread_mutex_lock(&mutex_);
        int ret = pthread_cond_timedwait(&cv_,&mutex_,&t_spec);
        if (ret < 0 && errno == ETIMEDOUT) {
          ret = 1;
        }

        pthread_mutex_unlock(&mutex_);

        return ret;
    }

    int signal(/*Thundering herd phenomenon safety*/) {

       pthread_mutex_lock(&mutex_);
       int ret = pthread_cond_signal(&cv_);
       pthread_mutex_unlock(&mutex_);

       return ret;
    }

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


    void init() {

      int ret = pthread_condattr_init(&cattr_);
      if (ret < 0) {
         make_core();
      }

      
      ret = pthread_cond_init(&cv_,&cattr_); 
      if (ret < 0) {
         make_core();
      }


      ret = pthread_mutex_init(&mutex_,NULL);
      if (ret < 0) {
         make_core();
      }
    }

    void destroy() {
       int ret = pthread_cond_destroy(&cv_);
       if (ret < 0) {}
       
       ret = pthread_mutex_destroy(&mutex_);
       if (ret < 0) {}

       ret = pthread_condattr_destroy(&cattr_);
       if (ret < 0) {}
    }
    
    pthread_cond_t cv_;
    pthread_mutex_t mutex_;
    pthread_condattr_t cattr_;

};

}
}












#endif  //__MONITOR_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
