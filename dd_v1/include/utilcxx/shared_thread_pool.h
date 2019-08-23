 
 
/**
 * @file thread_pool.h
 * @author lishilong(com@baidu.com)
 * @date 2013/08/02 19:38:51
 * @brief 
 *  
 **/




#ifndef  __SPEECH_UTIL_SHARED_THREAD_POOL_H_
#define  __SPEECH_UTIL_SHARED_THREAD_POOL_H_
#include <deque>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "atomic2speech.h"
#include "thread.h"
#include "task.h"
#include "recmutex.h"
#include "monitor.h"

namespace speech {
namespace util {

class SharedThreadPool;
typedef SharedThreadPool * SharedThreadPoolPtr;


class SharedTaskThread: public BaseThread {
public:
    SharedTaskThread():p_thpool_(NULL){}
    SharedTaskThread(SharedThreadPoolPtr p_thpool,const boost::shared_ptr<Task> & p_task,int timewait_times = 1);
    ~SharedTaskThread(){}
    virtual int run();
private:
    SharedTaskThread(const SharedTaskThread & );
    SharedTaskThread & operator =(const SharedTaskThread &);
    SharedThreadPool * p_thpool_;
    boost::shared_ptr<Task> p_task_;
    int timewait_times_;
};

class SharedThreadPool {
public:
    SharedThreadPool(unsigned int core_size/*ºËÐÄÏß³ÌÊý£¬³£×¤ÄÚ´æ*/,unsigned int max_size/*×î¸ßÏß³ÌÊýÏÞÖÆ*/,
               int channel/*·ÖÍ°*/,int timeo = 300/*Ïß³Ì¿ÕÏÐÊ±¼ä£¬³¬Ê±ÍË³ö*/);
    ~SharedThreadPool();
    int execute(const boost::shared_ptr<Task> & task);
    int addCoreThreadExecute(const boost::shared_ptr<Task> & task_ptr);
    int addMaxThreadExecute(const boost::shared_ptr<Task> & task_ptr);
    boost::shared_ptr<Task>  getTaskWithTimeWait();
    void threadCanExit(SharedTaskThread * task_thread_ptr);

    inline int getThreadRuningSize() {
        RecMutexLock::Lock lock(mutex_thread_pool_); 
        return thread_pool_.size() - idle_threads_.get();
    }

    inline int getThreadBlockingSize() {
        return idle_threads_.get();
    }

    inline int getThreadSize() {
        RecMutexLock::Lock lock(mutex_thread_pool_); 
        return thread_pool_.size();
    }

    inline int getTaskInQueueSize() {
        RecMutexLock::Lock lock(mutex_task_queue_);
        return task_queue_.size();
    }

private:
    AtomicInt  idle_threads_;
    unsigned int core_size_;
    unsigned int max_size_;
    int channel_;
    int timeo_;
    std::deque<boost::shared_ptr<Task> > task_queue_;
    std::map<SharedTaskThread *,int> thread_pool_;
    RecMutex mutex_task_queue_;
    RecMutex mutex_thread_pool_;
    Monitor monitor_;
};

}
}
















#endif  //__SHARED_THREAD_POOL_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
