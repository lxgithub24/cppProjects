 
 
/**
 * @file timer.h
 * @author lishilong(com@baidu.com)
 * @date 2013/09/03 14:55:19
 * @brief 
 *  
 **/




#ifndef  __SPEECH_UTIL_TIMER_H_
#define  __SPEECH_UTIL_TIMER_H_
#include<time.h>
#include<pthread.h>
namespace speech {
namespace util {
class Timer {
public:
    Timer(int channel,unsigned long interval,int repeat_times = -1):
        channel_(channel),interval_(interval),repeat_times_(repeat_times) {
    }

    int channel() {
       return channel_;
    }

    int repeat_times() {
       return repeat_times_;
    }

    unsigned long interval() {
       return interval_;
    }

    void runHandle() {
       pthread_testcancel(); 
       handle();
       pthread_testcancel(); 
    }
    virtual int handle() {return 0;};
protected:
    int channel_;
    unsigned long interval_;
    int repeat_times_;
};

typedef Timer * TimerPtr;

}
}














#endif  //__TIMER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
