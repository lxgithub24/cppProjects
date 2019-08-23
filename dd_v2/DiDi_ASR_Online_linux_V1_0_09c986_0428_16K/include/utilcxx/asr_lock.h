//
//  asr_lock.h
//  BDSASRCore
//
//  Created by baidu on 10/27/14.
//  Copyright (c) 2014 baidu. All rights reserved.
//

#ifndef __BDSASRCore__asr_lock__
#define __BDSASRCore__asr_lock__

#include <pthread.h>

namespace speech {
namespace asrcore {

class ASRLock
{
public:
    /**
     * @brief constructor
     *
     */
    ASRLock();
    
    /**
     * @brief destructor
     *
     */
    ~ASRLock();
    
    /**
     * @brief lock
     *
     */
    void lock();
    
    /**
     * @brief unlock
     *
     */
    void unlock();
    
private:
    /**
     * @brief copy constructor
     *
     */
    ASRLock(const ASRLock& left);
    
    /**
     * @brief assignment operator
     *
     */
    const ASRLock& operator = (const ASRLock& left);
    
private:
    pthread_mutex_t _lock;      /** lock instance */

    friend class ASRCondition;
};
   
class LockGuard
{
public:
    /**
     * @brief constructor, lock
     *
     */
    explicit LockGuard(ASRLock& left) : _asr_lock(left)
    {
        _asr_lock.lock();
    }
    
    /**
     * @brief destructor, unlock
     *
     */
    ~LockGuard()
    {
        _asr_lock.unlock();
    }
    
private:
    /**
     * @brief constructor, only declare
     *
     */
    LockGuard();
    
    /**
     * @brief copy constructor, only declare
     *
     */
    LockGuard(const LockGuard& left);
    
    /**
     * @brief assignment operator, only declare
     *
     */
    const LockGuard& operator = (const LockGuard& left);
    
private:
    ASRLock& _asr_lock;
};

class ASRCondition
{
public:
    /**
     * @brief constructor
     *
    **/
    ASRCondition();

    /**
     * @brief destructor
     *
    **/
    ~ASRCondition();

    /**
     * @brief wait to be signaled
     *
    **/
    void wait(ASRLock& lock);

    /**
     * @brief signal for waiting condition
     *
    **/
    void signal();

    /**
     * @brief broadcast to all conditions
     *
    **/
    void broadcast();

private:
    /**
     * @brief copy constructor, only declare
     *
    **/
    ASRCondition(const ASRCondition& left);

    /**
     * @brief assignment operator, only declare
     *
    **/
    ASRCondition& operator = (const ASRCondition& left);

private:
    pthread_cond_t _cond;          /**< condition */
};
}
} // namespace

#endif /* defined(__BDSASRCore__asr_lock__) */
