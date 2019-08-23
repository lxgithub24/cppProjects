

#ifndef  __MY_ATOMIC_H_
#define  __MY_ATOMIC_H_
#include "recmutex.h"
namespace speech {
namespace util {
typedef struct {
    int counter;
    RecMutex mutex_;
} my_atomic_t;

static inline int atomic_read(my_atomic_t * at) {
     RecMutexLock::Lock lock(at->mutex_);
     return at->counter;
}

static inline void atomic_set(my_atomic_t * at,int v) {
     RecMutexLock::Lock lock(at->mutex_);
     at->counter = v;
}

static inline void atomic_inc(my_atomic_t * at) {
    RecMutexLock::Lock lock(at->mutex_);
    at->counter+=1;
}

static inline void atomic_dec(my_atomic_t * at) {
    RecMutexLock::Lock lock(at->mutex_);
    at->counter-=1;
}

static inline bool atomic_dec_and_test(my_atomic_t * at) {
   RecMutexLock::Lock lock(at->mutex_);
   at->counter-=1;
   return at->counter == 0? true:false;
}

static inline void atomic_add(my_atomic_t * at,int v)  {
   RecMutexLock::Lock lock(at->mutex_);
   at->counter+=v;
}

static inline void atomic_min(my_atomic_t * at,int v)  {
   RecMutexLock::Lock lock(at->mutex_);
   at->counter-=v;
}

}
}
















#endif  //__MY_ATOMIC_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
