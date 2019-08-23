//

#ifndef BDSASRCore_asr_thread_h
#define BDSASRCore_asr_thread_h

#include <stdint.h>
#include <pthread.h>
#include "asr_lock.h"

namespace speech {
namespace asrcore {

class ASRThread
{
public:
    /**
     * @brief constructor
     *
     */
    ASRThread() : _is_stop(true), _is_detach(false)
    {
#ifndef __WINDOWS__
        _thread_handle = (pthread_t)NULL;
#endif
    }
    
    /**
     * @brief distructor
     *
     */
    virtual ~ASRThread()
    {
        detach();
    }
    
    /**
     * @brief 
     *
     */
    int32_t start();
    
    /**
     * @brief 
     *
     *
     */
    int32_t stop();
    
    /**
     * @brief 
     *
     */
    int32_t detach();
    
    /**
     * @brief 
     *
     */
    inline bool is_stop() const
    {
        return _is_stop;
    }
    
    /**
     * @brief 
     *
     */
    virtual void handle() = 0;
    
private:
    /**
     * @brief copy constructor
     *
     */
    ASRThread(ASRThread& left);
    
    /**
     * @brief assignment operator
     *
     */
    const ASRThread& operator = (const ASRThread& left);
    
    /**
     * @brief 
     *
     */
    static void* run(void* param);
    
private:
    bool _is_stop;
    bool _is_detach;
    pthread_t _thread_handle;
    ASRLock _lock;
};
        
} // namespace asrcore
} // namespace speech

#endif
