#ifndef THREADAPI_LINUXTHREAD_H
#define THREADAPI_LINUXTHREAD_H

#include <cstring>
#include <typeinfo>
#include <functional>

namespace rau
{

class LinuxThread
{
private:
    using CallableVoidWrapper = std::function<void()>;

public:
    using native_handle_type = pthread_t;

public:

    // The class LinuxThread::id is a lightweight, trivially copyable class that serves as
    // a unique identifier of LinuxThread.
    //
    // Instances of this class may also hold the special distinct value that
    // does not represent any Thread. Once a Thread has finished,
    // the value of LinuxThread::id may be reused by another Thread.
    //
    // This class is designed for use as key in associative containers, both ordered and unordered.
    class id
    {
        native_handle_type m_threadID{};
    public:
        friend bool operator==( id lhs, id rhs ) noexcept
        {
            return 0 != pthread_equal(lhs.m_threadID, rhs.m_threadID);
        }

        friend bool operator!=( id lhs, id rhs ) noexcept
        {
            return !(lhs == rhs);
        }

        friend bool operator< ( id lhs, id rhs ) noexcept
        {
            return lhs.m_threadID < rhs.m_threadID;
        }

        friend bool operator<=( id lhs, id rhs ) noexcept
        {
            return lhs.m_threadID <= rhs.m_threadID;
        }

        friend bool operator> ( id lhs, id rhs ) noexcept
        {
            return lhs.m_threadID > rhs.m_threadID;
        }

        friend bool operator>=( id lhs, id rhs ) noexcept
        {
            return lhs.m_threadID >= rhs.m_threadID;
        }

        friend std::ostream& operator<<(std::ostream& out, id obj)
        {
            out<<obj.m_threadID;
            return out;
        }

        friend std::hash<LinuxThread::id>;

    };

private:
    id m_ID;
    CallableVoidWrapper m_wrapper;
public:

    native_handle_type native_handle()
    {
        return *reinterpret_cast<native_handle_type*>(&m_ID);
    }

    LinuxThread() noexcept = default;

    template <typename Callable, typename... Args>
    explicit LinuxThread(Callable&& c, Args&&... args) {
        static_assert(std::is_constructible_v<std::decay_t<Callable>, Callable>,
                      "Callable must be constructible from the given type");
        static_assert((std::is_constructible_v<std::decay_t<Args>, Args> && ...),
                      "Each argument must be constructible from the given type");
        static_assert(std::is_invocable_v<std::decay_t<Callable>, std::decay_t<Args>...>,
                      "Callable must be invocable with the given arguments");

        m_wrapper = [func = std::forward<Callable>(c), tupleArgs = std::make_tuple(std::forward<Args>(args)...)]() mutable {
            std::apply(func, std::move(tupleArgs));
        };

        if (pthread_create(reinterpret_cast<pthread_t*>(&m_ID), nullptr, pthreadRoutine, &m_wrapper) != 0) {
            throw std::runtime_error("Failed to create thread");
        }
    }


    LinuxThread( LinuxThread&& other ) noexcept
    {
        if (joinable())
        {
            std::terminate();
        }

        m_ID = other.m_ID;
        m_wrapper = std::move(other.m_wrapper);
        other.m_ID = id{};
    }

    LinuxThread( const LinuxThread& ) = delete;

    ~LinuxThread()
    {
        if (joinable())
        {
            std::terminate();
        }
    }

    LinuxThread& operator=(LinuxThread&& other) noexcept {
        if (this != &other)
        {
            if (joinable())
            {
                std::terminate();
            }
            m_ID = other.m_ID;
            m_wrapper = std::move(other.m_wrapper);
            other.m_ID = id{};
        }
        return *this;
    }

    LinuxThread& operator=(LinuxThread& other) = delete;

    [[nodiscard]]
    id get_id() const
    {
        return m_ID;
    }

    [[nodiscard]]
    bool joinable() const
    {
        return get_id() != LinuxThread::id();
    }

    void join()
    {
        pthread_join(native_handle(), nullptr);
    }

    void detach()
    {
        pthread_detach(native_handle());
    }

private:
    static void* pthreadRoutine(void* routinePtr)
    {
        auto routine =
                *static_cast<CallableVoidWrapper*>(routinePtr);
        routine();

        return nullptr;
    }

};

}

namespace std
{

template <>
struct hash<rau::LinuxThread::id>
{
    size_t operator()(rau::LinuxThread::id&& id)
    {
        return id.m_threadID;
    }
};

} // namespace std


#endif //THREADAPI_LINUXTHREAD_H
