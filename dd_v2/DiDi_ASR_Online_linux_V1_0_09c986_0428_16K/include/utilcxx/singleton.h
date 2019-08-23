 
/**
 * @file singleton.h
 * @author lishilong(com@baidu.com)
 * @date 2013/08/17 16:34:10
 * @brief 
 *  
 **/




#ifndef  __SPEECH_UTIL_SINGLETON_H_
#define  __SPEECH_UTIL_SINGLETON_H_
#include<assert.h>
#include"recmutex.h"

namespace speech {
namespace util {

template <typename T> 
class Singleton {
public:
   
    static  T * instance() {
        
        if(obj_instance == NULL)
        {
            RecMutexLock::Lock lock(mutex_);
            if(obj_instance == NULL)
            {
                obj_instance = new T;
            }
        }

        assert(obj_instance != NULL);

        return obj_instance;
    }

protected:

    Singleton () {}

    virtual ~Singleton() {}

private:

    Singleton (const Singleton &) ;

    Singleton & operator = (const Singleton &);

    static RecMutex mutex_;
    static T * obj_instance;
};

template <typename T> T* Singleton<T>::obj_instance = NULL ;
template <typename T> RecMutex Singleton<T>::mutex_;

}
}
#endif  //__SINGLETON_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
