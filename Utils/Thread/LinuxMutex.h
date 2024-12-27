#ifndef THREADAPI_LINUXMUTEX_H
#define THREADAPI_LINUXMUTEX_H
#include <pthread.h>

namespace rau
{
// The LinuxMutex class is a synchronization primitive that can be used to protect
// shared data from being simultaneously accessed by multiple threads.

// LinuxMutex offers exclusive, non-recursive ownership semantics:
// 1. A calling Thread owns a LinuxMutex from the time that it successfully
//    calls either lock or try_lock until it calls unlock.
// 2. When a Thread owns a LinuxMutex, all other threads will block (for calls to lock)
//    or receive a false return value (for try_lock) if they attempt to claim ownership of the mutex.
// 3. A calling Thread must not own the mutex prior to calling lock or try_lock.
class LinuxMutex
{
public:
    using native_handle_type = pthread_mutex_t;
public:
    constexpr LinuxMutex() noexcept
    {
        // TODO
        // Constructs the mutex. The mutex is in unlocked state after the constructor completes.
    }

    LinuxMutex( const LinuxMutex& ) = delete;

    LinuxMutex& operator=(const LinuxMutex&) = delete;

    ~LinuxMutex()
    {
        // TODO
        // Destroys the mutex. The behavior is undefined if the mutex is owned by
        // any Thread or if any Thread terminates while holding any ownership of the mutex.
    }

    native_handle_type native_handle() const noexcept
    {
        // TODO
        return {};
    }

    void lock()
    {
        // TODO
        // Locks the mutex. If another Thread has already locked the mutex, a call
        // to lock will block execution until the lock is acquired.
        // If lock is called by a Thread that already owns the mutex, the behavior
        // is undefined: for example, the program may deadlock.
    }

    void unlock()
    {
        // TODO
        // Unlocks the mutex. The mutex must be locked by the current
        // Thread of execution, otherwise, the behavior is undefined.
    }

    bool try_lock()
    {
        // TODO
        // Tries to lock the mutex. Returns immediately. On successful lock acquisition
        // returns true, otherwise returns false.
        return {};
    }
};

// https://en.cppreference.com/w/cpp/thread/lock_guard
class LinuxMutexGuard
{
    // TODO
};

} // namespace rau

#endif //THREADAPI_LINUXMUTEX_H