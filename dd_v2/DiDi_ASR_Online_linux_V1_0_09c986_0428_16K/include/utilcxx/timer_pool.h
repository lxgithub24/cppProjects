 
 
/**
 * @file timer_pool.h
 * @author lishilong(com@baidu.com)
 * @date 2013/09/04 17:34:33
 * @brief 
 *  
 **/




#ifndef  __SPEECH_UTIL_TIMER_POOL_H_
#define  __SPEECH_UTIL_TIMER_POOL_H_
#include<sys/select.h>
#include<sys/types.h>
#include<unistd.h>
#include<map>

#include "thread.h"
#include "timer.h"
#include "singleton.h"
#include "recmutex.h"

namespace speech {
namespace util {

class TimerThPool;
typedef TimerThPool * TimerThPoolPtr;

class TimerThread :public BaseThread {
public:
    TimerThread(TimerThPoolPtr timer_pool_ptr,TimerPtr timer_ptr):
        timer_pool_ptr_(timer_pool_ptr),timer_ptr_(timer_ptr){
        
    }

    virtual int run();
private:
    TimerThPoolPtr timer_pool_ptr_;
    TimerPtr timer_ptr_;
};

class TimerThPool:public Singleton<TimerThPool> {
public:
    TimerThPool():max_interval_(0) {
    
    }

    bool execute(TimerPtr timer);
    void timerCanExit(int channel); 
    void smoothExit();
    int getTimerSize() {
       RecMutexLock::Lock lock(mutex_);
       return timer_thpool_.size();
    }
private:
    RecMutex mutex_; 
    std::map<int,TimerThread *> timer_thpool_;
    unsigned long max_interval_;

};

}
}
















#endif  //__TIMER_POOL_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
