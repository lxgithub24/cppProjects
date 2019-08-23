 
 
/**
 * @file task_runner.h
 * @author lishilong(com@baidu.com)
 * @date 2013/08/17 16:20:45
 * @brief 
 *  
 **/




#ifndef  __SPEECH_UTIL_TASK_RUNNER_H_
#define  __SPEECH_UTIL_TASK_RUNNER_H_
#include <utility>
#include <map>
#include <boost/shared_ptr.hpp>
#include "thread_pool.h"
#include "shared_thread_pool.h"
#include "timer_pool.h"
#include "singleton.h"

namespace speech {
namespace util {

typedef  std::map<int,std::pair<int,int> > ThreadPoolConfig;

//threads wake up per 1000ms
const int kThreadWaitingTaskTime = 1000;
const int kDefaultCoreSize = 10;
const int kDefaultMaxSize  = 100;
class TaskRunner:public Singleton<TaskRunner> {
public:
     TaskRunner(){} 
    ~TaskRunner() {
    
        for(std::map<int,ThreadPoolPtr>::iterator it = thpools_.begin(); it != thpools_.end();++it) {
            try {

              delete it->second;

            } catch (std::exception &e) {
            
            } catch (...) {
            
            }
        }
    }

    int threadPoolConfig(int channel,const std::pair<int,int> &);

    void execute(TaskPtr task);

    void execute(const boost::shared_ptr<Task> & task);

    bool scheduleRepeated(TimerPtr timer);

    void smoothExit();

    inline int getTaskInQueueSize(int channel) {
        RecMutexLock::Lock lock(mutex_);
        std::map<int,ThreadPoolPtr>::iterator itr = thpools_.find(channel);
        if (itr != thpools_.end()) {
            return itr->second->getTaskInQueueSize();
        }
        
        std::map<int,SharedThreadPoolPtr>::iterator its = shared_thpools_.find(channel);
        if (its != shared_thpools_.end()) {
           return its->second->getTaskInQueueSize();
        }
        return -1;
    }

    inline int getThreadRunningSize(int channel) {
        RecMutexLock::Lock lock(mutex_);
        std::map<int,ThreadPoolPtr>::iterator itr = thpools_.find(channel);
        if (itr != thpools_.end()) {
           return itr->second->getThreadRuningSize();
        }
        std::map<int,SharedThreadPoolPtr>::iterator its = shared_thpools_.find(channel);
        if (its != shared_thpools_.end()) {
           return its->second->getThreadRuningSize();
        }
        return -1;
    }

    inline int getThreadBlockingSize(int channel) {
        RecMutexLock::Lock lock(mutex_);
        std::map<int,ThreadPoolPtr>::iterator itr = thpools_.find(channel);
        if (itr != thpools_.end()) {
           return itr->second->getThreadBlockingSize();
        }
        std::map<int,SharedThreadPoolPtr>::iterator its = shared_thpools_.find(channel);
        if (its != shared_thpools_.end()) {
           return its->second->getThreadBlockingSize();
        }
        return -1;
    }

    inline int getThreadSize(int channel) {
        RecMutexLock::Lock lock(mutex_);
        std::map<int,ThreadPoolPtr>::iterator itr = thpools_.find(channel);
        if (itr != thpools_.end()) {
           return itr->second->getThreadSize();
        }
        std::map<int,SharedThreadPoolPtr>::iterator its = shared_thpools_.find(channel);
        if (its != shared_thpools_.end()) {
           return its->second->getThreadSize();
        }
        return -1;
    }

    void getThreadPoolConfig(std::map<int,std::pair<int,int> > & thpool_config) {
        RecMutexLock::Lock lock(mutex_);
        for (std::map<int,std::pair<int,int> >::iterator it = thpool_config_.begin();
                 it != thpool_config_.end(); ++it) {
            thpool_config.insert(std::map<int,std::pair<int,int> >::value_type(it->first,it->second));
        }
    }
    
private:
    TaskRunner(const  TaskRunner &);
    TaskRunner & operator = (const TaskRunner &);
    std::map<int,std::pair<int,int> > thpool_config_;
    std::map<int,ThreadPoolPtr> thpools_;
    std::map<int,SharedThreadPoolPtr> shared_thpools_;
    RecMutex mutex_;

};


}
}




#endif  //__TASK_RUNNER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
