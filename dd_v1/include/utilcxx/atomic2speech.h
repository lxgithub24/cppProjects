

#ifndef  __ATOMIC2SPEECH_H_
#define  __ATOMIC2SPEECH_H_

#include "comatomic.h"

namespace speech
{
namespace util 
{
	namespace 
	{
		template <typename T>
		struct AtomicTratis
		{
			typedef T type;
		};
		template <>
		struct AtomicTratis<int8_t>
		{
			typedef uint8_t type;
		};
		template <> 
		struct AtomicTratis<int16_t>
		{
			typedef uint16_t type;
		};
		template <>
		struct AtomicTratis<int32_t>
		{
			typedef uint32_t type;
		};
		template <>
		struct AtomicTratis<int64_t>
		{
			typedef uint64_t type;
		};
	}

	template <typename T>
	class Atomic
	{
		public:
			/**
			 * @brief 使用初始值 0 创建新的 Atomic。
			 *
			 * @see 
			 * @date 2009/04/15 18:17:14
			 **/
			inline Atomic() : _value(0) {}

			/**
			 * @brief 
			 *
			 * @param [in/out] initialValue   : T
			 * @see 
			 * @date 2009/04/15 18:17:08
			 **/
			inline Atomic(T initialValue) : _value(initialValue) {}

			/**
			 * @brief 以原子方式将给定值与当前值相加。
			 *
			 * @param [in/out] delta   : T
			 * @return  T	返回加之后的值 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 18:17:34
			 **/
			inline T 	addAndGet(T delta) { return getAndAdd(delta) + delta; }
			/**
			 * @brief  如果当前值 == 预期值，则以原子方式将该值设置为给定的更新值。
			 *
			 * @param [in/out] expect   : T
			 * @param [in/out] update   : T
			 * @return  bool 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 18:17:57
			 **/
			inline bool 	compareAndSet(T expect, T update) 
			{ 
				return atomic_compare_exchange(
						&_value, update, expect) == typename AtomicTratis<T>::type(expect); 
			}

			/**
			 * @brief 以原子方式将当前值减 1。
			 *
			 * @return  T 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 18:18:16
			 **/
			inline T 	decrementAndGet() { return atomic_dec(&_value); }

			/**
			 * @brief  以原子方式将当前值加 1。
			 *
			 * @return  T 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 18:19:07
			 **/
			inline T 	incrementAndGet() { return atomic_inc(&_value); }

			/**
			 * @brief 获取当前值。
			 *
			 * @return  T 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 18:18:23
			 **/
			inline T 	get() const { return _value; }

			/**
			 * @brief  设置为给定值。
			 *
			 * @param [in/out] newValue   : T
			 * @return  void 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 18:19:18
			 **/
			inline void 	set(T newValue) { _value = newValue; }

			/**
			 * @brief  以原子方式将给定值与当前值相加。
			 *
			 * @param [in/out] delta   : T
			 * @return  T 返回加之前的值
			 * @retval   
			 * @see 
			 * @date 2009/04/15 18:18:33
			 **/
			inline T 	getAndAdd(T delta) { return atomic_add (&_value, delta); }

			/**
			 * @brief  以原子方式将当前值减 1。
			 *
			 * @return  T 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 18:18:40
			 **/
			inline T 	getAndDecrement() { return decrementAndGet() + 1;}

			/**
			 * @brief  以原子方式将当前值加 1。
			 *
			 * @return  T 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 18:18:48
			 **/
			inline T 	getAndIncrement() { return incrementAndGet() - 1; }

			/**
			 * @brief 设置为给定值，并返回旧值。
			 *
			 * @param [in/out] newValue   : T
			 * @return  T 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 18:18:56
			 **/
			inline T 	getAndSet(T newValue) { return atomic_exchange (&_value, newValue); }

		private:
			typename AtomicTratis<T>::type  _value;
	};

	template <> class Atomic<bool> 
	{
		public:
			/**
			 * @brief 使用初始值 false 创建新的 AtomicBoolean
			 *
			 * @see 
			 * @date 2009/04/15 19:17:02
			**/
			inline Atomic() : _atomic(0) {}

			/**
			 * @brief  使用给定的初始值创建新的 AtomicBoolean
			 *
			 * @param [in/out] initialValue   : bool
			 * @see 
			 * @date 2009/04/15 19:17:19
			**/
			inline Atomic(bool initialValue) : _atomic(initialValue?1:0) {}

			/**
			 * @brief 如果当前值 == 预期值，则以原子方式将该值设置为给定的更新值。
			 *
			 * @param [in/out] expect   : bool
			 * @param [in/out] update   : bool
			 * @return  bool 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 19:17:31
			**/
			inline bool 	compareAndSet(bool expect, bool update)
			{
				return _atomic.compareAndSet(expect?1:0, update?1:0);
			}

			/**
			 * @brief  返回当前值。
			 *
			 * @return  bool 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 19:17:38
			**/
			inline bool 	get()
			{
				return _atomic.get() != 0;
			}

			/**
			 * @brief 设置为给定值，并返回以前的值。
			 *
			 * @param [in/out] newValue   : bool
			 * @return  bool 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 19:17:55
			**/
			inline bool 	getAndSet(bool newValue)
			{
				return _atomic.getAndSet(newValue?1:0) != 0;
			}

			/**
			 * @brief 无条件地设置为给定值。
			 *
			 * @param [in/out] newValue   : bool
			 * @return  void 
			 * @retval   
			 * @see 
			 * @date 2009/04/15 19:18:18
			**/
			inline void 	set(bool newValue) 
			{
				_atomic.set(newValue?1:0);
			}
		private:
			Atomic<int8_t> _atomic;
	};

	template <typename T> class Atomic<T *>
	{
		public:
			inline Atomic() : _ptr(0) {}
			inline Atomic(const T *ptr) : _ptr((ptr_t)ptr) {}
			inline bool compareAndSet(const T *expect, const T *update)
			{
				return _ptr.compareAndSet((ptr_t)expect, (ptr_t)update);
			}
			inline T * get()
			{
				return (T *) _ptr.get();
			}
			inline void set(const T *newValue)
			{
				_ptr.set((ptr_t)newValue);
			}

		private:
#ifdef __x86_64__
			typedef uint64_t ptr_t;
#else
			typedef uint32_t ptr_t;
#endif
			Atomic<ptr_t> _ptr;
	};

	typedef Atomic<int32_t> AtomicInt;
	typedef Atomic<int64_t> AtomicLong;
	typedef Atomic<bool> AtomicBool;
}
}

#endif  //__ATOMIC_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
