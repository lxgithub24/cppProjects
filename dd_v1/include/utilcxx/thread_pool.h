 
 
/**
 * @file thread_pool.h
 * @author lishilong(com@baidu.com)
 * @date 2013/08/02 19:38:51
 * @brief 
 *  
 **/




#ifndef  __SPEECH_UTIL_THREAD_POOL_H_
#define  __SPEECH_UTIL_THREAD_POOL_H_
#include <deque>
#include <vector>
#include <map>

#include "atomic2speech.h"
#include "thread.h"
#include "task.h"
#include "recmutex.h"
#include "monitor.h"

namespace speech {
namespace util{

class ThreadPool;
typedef Task * TaskPtr;
typedef ThreadPool * ThreadPoolPtr;

class TaskThread: public BaseThread {
public:
    TaskThread():p_thpool_(NULL),p_task_(NULL){}
    TaskThread(ThreadPoolPtr p_thpool,Task * p_task = NULL,int timewait_times = 1);
    ~TaskThread(){}
    virtual int run();
    void freeObject(TaskPtr * task_ptr);
private:
    TaskThread(const TaskThread & );
    TaskThread & operator =(const TaskThread &);
    ThreadPool * p_thpool_;
    Task * p_task_;
    int timewait_times_;
};

class ThreadPool {
public:
    ThreadPool(unsigned int core_size/*ºËÐÄÏß³ÌÊý£¬³£×¤ÄÚ´æ*/,unsigned int max_size/*×î¸ßÏß³ÌÊýÏÞÖÆ*/,
               int channel/*·ÖÍ°*/,int timeo = 300/*Ïß³Ì¿ÕÏÐÊ±¼ä£¬³¬Ê±ÍË³ö*/);
    ~ThreadPool();
    int execute(TaskPtr task);
    int addCoreThreadExecute(TaskPtr task_ptr);
    int addMaxThreadExecute(TaskPtr task_ptr);
    TaskPtr getTaskWithTimeWait();
    void threadCanExit(TaskThread * task_thread_ptr);

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
    AtomicInt idle_threads_;
    unsigned int core_size_;
    unsigned int max_size_;
    int channel_;
    int timeo_;
    std::deque<TaskPtr> task_queue_;
    std::map<TaskThread *,int> thread_pool_;
    RecMutex mutex_task_queue_;
    RecMutex mutex_thread_pool_;
    Monitor monitor_;

};

}
}
















#endif  //__THREAD_POOL_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
