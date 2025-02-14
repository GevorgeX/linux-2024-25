#pragma once
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

template <class  T>
class BlockingQueue {
    std::queue<T> m_queue;
    std::mutex m_mtx;
    std::condition_variable m_cond_var;
    std::condition_variable m_done_wait;
    const size_t m_max_size;
    bool m_done = false;
    int m_thread_count = 0;

public:
    explicit BlockingQueue(size_t size):m_max_size(size) {
    }

    ~BlockingQueue()
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_done = true;

        m_cond_var.notify_all();
        m_done_wait.wait(lock, [&]() {
                    return m_thread_count == 0;
                });
    }

    void push(const T& t) noexcept  {
        std::unique_lock<std::mutex> lock{m_mtx};
        m_thread_count++;
        m_cond_var.wait(lock,
                      [this] () { return m_done || m_queue.size() < m_max_size; }
                      );
        m_queue.push(t);

        m_thread_count--;
        if (m_thread_count == 0) {
            m_done_wait.notify_one();
        }
        lock.unlock();
        m_cond_var.notify_one();
    }

    void push(T&& t) noexcept  {
        std::unique_lock<std::mutex> lock{m_mtx};
        m_thread_count++;
        m_cond_var.wait(lock,
                      [this] () { return m_done || m_queue.size() < m_max_size; }
                      );
        m_queue.push(t);

        m_thread_count--;
        if (m_thread_count == 0) {
            m_done_wait.notify_one();
        }
        lock.unlock();
        m_cond_var.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_thread_count++;
        m_cond_var.wait(lock, [this] { return m_done || !m_queue.empty(); });
        auto value = m_queue.front();
        m_queue.pop();

        m_thread_count--;
        if (m_thread_count == 0) {
            m_done_wait.notify_one();
        }
        lock.unlock();
        m_cond_var.notify_one();

        return value;
    }

    bool try_push(const T& t) noexcept
    {
        std::unique_lock<std::mutex> lock{m_mtx};
        if(m_queue.size() >= m_max_size)
        {
            return false;
        }
        m_queue.push(t);
        return true;
    }

    bool try_push(T&& t) noexcept
    {
        std::unique_lock<std::mutex> lock{m_mtx};
        if(m_queue.size() >= m_max_size)
        {
            return false;
        }
        m_queue.push(t);
        return true;
    }

    std::optional<T> try_pop()
    {
        std::unique_lock<std::mutex> lock{m_mtx};
        if(m_queue.empty())
        {
            return std::nullopt;
        }
        auto value = m_queue.front();
        m_queue.pop();
        return value;
    }

    size_t size() noexcept {
        std::unique_lock<std::mutex> lock{m_mtx};
        return m_queue.size();
    }

};
