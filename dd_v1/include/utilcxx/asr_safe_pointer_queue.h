#ifndef __BDSASRCore__asr_safe_list__
#define __BDSASRCore__asr_safe_list__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "asr_lock.h"
#include "asr_log.h"

namespace speech {
namespace asrcore {
        
template <typename T, uint32_t DEFAULT_ELEMENT_SIZE = 20>
class ASRSafePointerQueue
{
public:
    /**
     * @brief constructor
     *
     */
    ASRSafePointerQueue():_head(0), _tail(0), _element_size(DEFAULT_ELEMENT_SIZE), _buf(NULL)
    {
        _buf = (T**)malloc(sizeof(T*) * _element_size);
        if (NULL == _buf) {
            BDS_LOG_FATAL("[ASRSafePointerQueue::ASRSafePointerQueue] malloc for _buf failed");
            abort();
        }
        memset(_buf, 0, sizeof(T*) * _element_size);
    }

    /**
     * @brief destructor
     *
     */
    ~ASRSafePointerQueue()
    {
        LockGuard guard(_lock);
        if (NULL != _buf) {
            while (_head != _tail) {
                delete _buf[_head];
                _buf[_head] = NULL;
                _head = ++_head % _element_size;
            }
            free(_buf);
            _buf = NULL;
            _element_size = 0;
        }
    }
    
    /**
     * @brief insert data
     *
     */
    void push(T* node)
    {
        LockGuard guard(_lock);
        uint32_t next_free_pos = (_tail + 1) % _element_size;
        if (next_free_pos == _head) {
            adjust_capacity();
            next_free_pos = (_tail + 1) % _element_size;
        }
        
        _buf[_tail] = node;
        _tail = next_free_pos;
    }
    
    /**
     * @brief pop data
     *
     */
    T* pop()
    {
        LockGuard guard(_lock);
        if (_head == _tail) {
            return NULL;
        }
        
        T* node = _buf[_head];
        _buf[_head] = NULL;
        _head = (_head + 1) % _element_size;
        
        return node;
    }
    
    /**
     * @brief remove element
     *
     */
    bool remove(T* pointer)
    {
        if (NULL == pointer) {
            return false;
        }
        
        LockGuard guard(_lock);
        
        // empty list
        if (_head == _tail) {
            return false;
        }
        
        if (pointer == _buf[_head]) {
            _buf[_head] = NULL;
            _head = (_head + 1) % _element_size;
            return true;
        }
        
        uint32_t tmp = (_head + 1) % _element_size;
        while (pointer != _buf[tmp] && tmp != _tail) {
            tmp = (tmp + 1) % _element_size;
        }

        if (tmp != _tail) {
            if ((tmp + 1) % _element_size == _tail) { 
                _tail = (_tail + _element_size - 1) % _element_size;
            } else if (tmp < _tail) {
                memcpy(_buf + tmp, _buf + tmp + 1, (_tail - tmp - 1) * sizeof(T*));
                _tail = (_tail + _element_size - 1) % _element_size;
            } else {
                memcpy(_buf + _head + 1, _buf + _head, (tmp - _head) * sizeof(T*));
                _head = _head + 1;
            }
            
            return true;
        }
        
        return false;
    }
    
    /**
     * @brief get count
     *
     */
    size_t count()
    {
        LockGuard guard(_lock);
        return ((_tail + _element_size - _head) % _element_size);
    }
    
    /**
     * @brief clear list
     *
     */
    void clear()
    {
        LockGuard guard(_lock);
        BDS_LOG_DEBUG("[ASRSafePointerQueue::clear] %d items will be cleared",
                      (_tail + _element_size - _head) % _element_size);
        if (NULL != _buf) {
            while (_head != _tail) {
                delete _buf[_head];
                _buf[_head] = NULL;
                _head = ++_head % _element_size;
            }
            _head = _tail = 0;
        }
    }
    
private:
    /**
     * @brief copy constructor, only declare
     *
     */
    ASRSafePointerQueue(const ASRSafePointerQueue& left);
    
    /**
     * @brief assignment operator
     *
     */
    const ASRSafePointerQueue& operator = (const ASRSafePointerQueue& left);
    
    /**
     * @brief adjust list capacity
     *
     */
    void adjust_capacity()
    {
        T** tmp = (T**)malloc(sizeof(T*) * _element_size * 2);
        if (NULL == tmp) {
            BDS_LOG_FATAL("[ASRSafePointerQueue::adjust_capacity] malloc for _buf failed");
            abort();
        }
        
        if (_head == _tail) { 
            //
        } else if (_head < _tail) {
            memcpy(tmp, _buf, (_tail - _head) * sizeof(T*));
        } else {
            memcpy(tmp, _buf + _head, (_element_size - _head) * sizeof(T*));
            memcpy(tmp + (_element_size - _head), _buf, (_tail) * sizeof(T*));
        }
        
        _tail = (_tail + _element_size - _head) % _element_size;
        _head = 0;
        
        free(_buf);
        _buf = tmp;
        _element_size *= 2;
    }
    
private:
    uint32_t    _head;          
    uint32_t    _tail;          
    uint32_t    _element_size;  
    T**         _buf;           
    ASRLock     _lock;
};
        
} // namespace asrcore
} // namespace speech

#endif /* defined(__BDSASRCore__asr_safe_list__) */
