

#ifndef __BDSASRCore__asr_thread_pool__
#define __BDSASRCore__asr_thread_pool__

#include <stdio.h>
#include "asr_safe_pointer_queue.h"
#include "asr_thread.h"
#include "asr_log.h"

namespace speech {
namespace asrcore {
        
class ASRDispatchThread;
class ASRDispatchQueueBase
{
public:
    /**
     * @brief contructor
     *
     */
    ASRDispatchQueueBase()
    {
    }
    
    /**
     * @brief destructor
     *
     */
    virtual ~ASRDispatchQueueBase()
    {
    }
    
    /**
     * @brief handle function for thread pool
     *
     */
    virtual void handle(ASRDispatchThread* thread)
    {
        thread = thread;
    }
    
private:
    /**
     * @brief copy constructor, only declare
     *
     */
    ASRDispatchQueueBase(const ASRDispatchQueueBase& left);
    
    /**
     * @brief assignment operator, only declare
     *
     */
    const ASRDispatchQueueBase& operator = (const ASRDispatchQueueBase& left);
};

// dispatch threads
class ASRDispatchThread : public ASRThread
{
public:
    /**
     * @brief constructor
     *
     */
    explicit ASRDispatchThread(ASRDispatchQueueBase* threa_pool)
    : _thread_pool(threa_pool)
    {
    }
    
    /**
     * @brief destructor
     *
     */
    virtual ~ASRDispatchThread()
    {
    }
    
    /**
     * @brief thread run function.
     *
     */
    virtual void handle()
    {
        if (_thread_pool) {
            _thread_pool->handle(this);
        } else {
            BDS_LOG_ERROR("[ASRDispatchThread::handle] thread pool is NULL");
        }
    }
    
private:
    ASRDispatchQueueBase* _thread_pool;
};

// asr task base class
class ASRDispatchTaskBase
{
public:
    /**
     * @brief constructor
     *
    **/
    ASRDispatchTaskBase()
    {
    }

    /**
     * @brief distructor
     *
    **/
    virtual ~ASRDispatchTaskBase()
    {
    }

    /**
     * @brief execute task
     *
    **/
    virtual void execute()
    {
    }

private:
    /**
     * @brief copy consturctor, only declare
     *
    **/
    ASRDispatchTaskBase(const ASRDispatchTaskBase& left);

    /**
     * @brief assignment operator 
     *
    **/
    ASRDispatchTaskBase& operator =(const ASRDispatchTaskBase& left);
};

// disptach task with resource pointer
template <typename TCLASS, typename RESOURCE>
class ASRDispatchTaskWithResPointer : public ASRDispatchTaskBase
{
public:
    // Class function type
    typedef void (TCLASS::*process_fuc)(RESOURCE* resource);
    /**
     * @brief constructor
     *
     */
    ASRDispatchTaskWithResPointer():_instance(NULL), _fuc(NULL), _resource(NULL)
    {
    }

    /**
     * @brief constructor
     *
     */
    ASRDispatchTaskWithResPointer(TCLASS* instance, process_fuc fuc, RESOURCE* resource)
        :_instance(instance), _fuc(fuc), _resource(resource)
    {
    }

    /**
     * @brief destructor
     *
     */
    virtual ~ASRDispatchTaskWithResPointer()
    {
        if (NULL != _resource) {
            delete _resource;
            _resource = NULL;
        }
    }

    /**
     * @brief execute task
     *
    **/
    virtual void execute()
    {
        (_instance->*_fuc)(_resource);
        _resource = NULL; // runned task resource released by external.
    }

private:
    /**
     * @brief copy constructor, only declare
     *
     */
    ASRDispatchTaskWithResPointer(const ASRDispatchTaskWithResPointer& left);

    /**
     * @brief assignment operator, only declare
     *
     */
    const ASRDispatchTaskWithResPointer operator = (const ASRDispatchTaskWithResPointer& left);

public:
    TCLASS*     _instance;
    process_fuc _fuc;
    RESOURCE*   _resource;
};

// dispatch task with resource instance
template <typename TCLASS, typename RESOURCE>
class ASRDispatchTaskWithResInstance : public ASRDispatchTaskBase
{
public:
    // Class function type
    typedef void (TCLASS::*process_fuc)(RESOURCE& resource);
    /**
     * @brief constructor
     *
    **/
    ASRDispatchTaskWithResInstance()
    {
    }

    /**
     * @brief constructor
     *
    **/
    ASRDispatchTaskWithResInstance(TCLASS* instance, process_fuc fuc, RESOURCE& res) 
    : _instance(instance), _fuc(fuc), _resource(res)
    {
    }

    /**
     * @brief execute task
     *
    **/
    virtual void execute()
    {
        (_instance->*_fuc)(_resource);
    }

    /**
     * @brief destructor
     *
    **/
    virtual ~ASRDispatchTaskWithResInstance()
    {
    }

private:
    /**
     * @brief copy constructor, only declare
     *
    **/
    ASRDispatchTaskWithResInstance(const ASRDispatchTaskWithResInstance& left);

    /**
     * @brief assignment operator, only declare
     *
    **/
    ASRDispatchTaskWithResInstance& operator = (const ASRDispatchTaskWithResInstance& left);

public:
    TCLASS*     _instance;
    process_fuc _fuc;
    RESOURCE       _resource;
};

// dispatch task without resource
template <typename TCLASS>
class ASRDispatchTaskNoRes : public ASRDispatchTaskBase
{
public:
    // Class function type
    typedef void (TCLASS::*process_fuc)();
    /**
     * @brief constructor
     *
    **/
    ASRDispatchTaskNoRes():_instance(NULL), _fuc(NULL)
    {
    }

    /**
     * @brief constructor
     *
    **/
    ASRDispatchTaskNoRes(TCLASS* instance, process_fuc fuc) : _instance(instance), _fuc(fuc)
    {
    }

    /**
     * @brief distructor
     *
    **/
    virtual ~ASRDispatchTaskNoRes()
    {
    }

    virtual void execute()
    {
        (_instance->*_fuc)();
    }
private:
    /**
     * @brief copy constructor, only declare
     *
    **/
    ASRDispatchTaskNoRes(const ASRDispatchTaskNoRes& left);

    /**
     * @brief assignment operator, only declare
     *
    **/
    ASRDispatchTaskNoRes& operator = (const ASRDispatchTaskNoRes& left);

public:
    TCLASS*     _instance;
    process_fuc _fuc;
};
    
// Asynchronous thread pool
class ASRDispatchQueue : public ASRDispatchQueueBase
{
public:
    /**
     * @brief constructor
     *
     */
    ASRDispatchQueue():_is_cancel(false), _thread_concurrent_count(1), _thread_idle_count(0)
    {
    }
    
    /**
     * @brief destructor
     *
     */
    virtual ~ASRDispatchQueue()
    {
        cancel_all_task();
    }
    
    /**
     * @brief Use thread pool to Asynchronous
     *
     */
    template <typename TCLASS, typename RESOURCE, typename CLASSMEMFUNC>
    void asyn_execute(TCLASS* instance, CLASSMEMFUNC fuc, RESOURCE* resource)
    {
        if (NULL == instance || NULL == fuc) {
            BDS_LOG_ERROR("[ASRDispatchQueue::asyn_execute] instance or fuc is NULL");
            return;
        }
        
        ASRDispatchTaskBase* task = new ASRDispatchTaskWithResPointer<TCLASS, RESOURCE>(instance,
                fuc, resource);
        if (NULL == task) {
            BDS_LOG_FATAL("[ASRDispatchQueue::asyn_execute] new ASRDispatchTaskWithResPointer"
                    " failed, abort!");
            abort();
        }

        push_into_task_queue(task);
    }
   
    /**
     * @brief Use thread pool to Asynchronous
     *
     */
    template <typename TCLASS, typename RESOURCE, typename CLASSMEMFUNC>
    void asyn_execute(TCLASS* instance, CLASSMEMFUNC fuc, RESOURCE& resource)
    {
        if (NULL == instance || NULL == fuc) {
            BDS_LOG_ERROR("[ASRDispatchQueue::asyn_execute] instance or fuc is NULL");
            return;
        }
        
        ASRDispatchTaskBase* task = new ASRDispatchTaskWithResInstance<TCLASS, RESOURCE>(instance, fuc, resource);
        if (NULL == task) {
            BDS_LOG_FATAL("[ASRDispatchQueue::asyn_execute] new ASRDispatchTaskWithInstance failed, abort!");
            abort();
        }

        push_into_task_queue(task);
    }

    /**
     * @brief Use thread pool to Asynchronous
     *
     */
    template <typename TCLASS, typename CLASSMEMFUNC>
    void asyn_execute(TCLASS* instance, CLASSMEMFUNC fuc)
    {
        if (NULL == instance || NULL == fuc) {
            BDS_LOG_ERROR("[ASRDispatchQueue::asyn_execute] instance or fuc is NULL");
            return;
        }
        
        ASRDispatchTaskBase* task = new ASRDispatchTaskNoRes<TCLASS>(instance, fuc);
        if (NULL == task) {
            BDS_LOG_FATAL("[ASRDispatchQueue::asyn_execute] new ASRDispatchTaskNoRes failed, abort!");
            abort();
        }

        push_into_task_queue(task);
    }

    /**
     * @brief cancel all tasks
     *
     */
    void cancel_all_task()
    {
        {
            LockGuard guard(_lock);
            _is_cancel = true;
            _cond.broadcast();
        }
        
        _task_list.clear();
        ASRDispatchThread* thread = NULL;
        while (NULL != (thread = _thread_queue.pop())) {
            thread->stop();
            delete thread;
            thread = NULL;
        }
        
        {
            LockGuard guard(_lock);
            _is_cancel = false;
        }
    }
    
    /**
     * @brief remove all waiting task
     *
     */
    void remove_wait_tasks()
    {
        _task_list.clear();
    }
    
    /**
     * @brief set thread pool number
     *
     */
    void set_thread_concurrent_count(uint32_t count)
    {
        _thread_concurrent_count = count;
    }

    /**
     * @brief get task counts
     *
    **/
    uint32_t get_task_count()
    {
        return _task_list.count();
    }

    /**
     * @brief get idle thread count
     *
    **/
    uint32_t get_idle_thread_count()
    {
        uint32_t count = 0;
        {
            LockGuard guard(_lock);
            count = _thread_idle_count;
        }

        return count;
    }

    /**
     * @brief get total thread count
     *
    **/
    uint32_t get_total_thread_count()
    {
        uint32_t count = 0;
        {
            LockGuard guard(_lock);
            count = _thread_queue.count();
        }

        return count;
    }
    
    /**
     * @brief get thread concurrent count
     *
    **/
    uint32_t get_thread_concurrent_count() const
    {
        return _thread_concurrent_count;
    }

private:
    /**
     * @brief copy constructor, only declare
     *
     */
    ASRDispatchQueue(const ASRDispatchQueue& left);
    
    /**
     * @brief assignment operator, only declare
     *
     */
    const ASRDispatchQueue& operator = (const ASRDispatchQueue& left);

    /**
     * @brief push task into queue
     *
    **/
    void push_into_task_queue(ASRDispatchTaskBase* task)
    {
        if (NULL == task) {
            BDS_LOG_ERROR("[ASRDispatchQueue::push_into_task_queue] task is NULL");
            return;
        }

        LockGuard guard(_lock);
        
        if (_is_cancel) {
            BDS_LOG_ERROR("[ASRDispatchQueue::asyn_execute] resource thread pool is cancelled");
            if (NULL != task) {
                delete task;
                task = NULL;
            }
            return;
        }
        
        _task_list.push(task);
        
        if (0 < _thread_idle_count) {
            _cond.signal();
        } else if (_thread_concurrent_count > _thread_queue.count()) {
            ASRDispatchThread* thread = new ASRDispatchThread(this);
            if (NULL == thread) {
                BDS_LOG_FATAL("[ASRDispatchQueue::asyn_execute] new ASRDispatchThread failed, abort!");
                abort();
            }
            
            _thread_queue.push(thread);
            thread->start();
        }
    }
    
    /**
     * @brief run function of thread pool thread.
     *
     */
    void handle(ASRDispatchThread* thread)
    {
        ASRDispatchTaskBase* task = NULL;
        while (true) {
            {
                LockGuard guard(_lock);
                if (_is_cancel) {
                    BDS_LOG_DEBUG("[ASRDispatchQueue::handle] thread pool cancel");
                    break;
                }
                
                if (0 == _task_list.count()) {
                    ++_thread_idle_count;
                    _cond.wait(_lock);
                    --_thread_idle_count;
                }

                task = _task_list.pop();
                if (NULL == task) {
//                    if (_thread_queue.remove(thread)) {
//                        thread->detach();
//                        delete thread;
//                    }
//
//                    break;
                    thread = thread;
                    continue;
                }
            }

            task->execute();    
            delete task;
            task = NULL;
        }
        
        BDS_LOG_DEBUG("[ASRDispatchQueue::handle] thread finish");
    }

private:
    bool _is_cancel;
    uint32_t _thread_concurrent_count;
    uint32_t _thread_idle_count;
    ASRLock _lock;
    ASRCondition _cond;
    ASRSafePointerQueue<ASRDispatchTaskBase> _task_list;
    ASRSafePointerQueue<ASRDispatchThread> _thread_queue;

    friend class ASRDispatchThread;
};

} // namesapce asrcore
} // namespace speech

#endif /* defined(__BDSASRCore__asr_thread_pool__) */
