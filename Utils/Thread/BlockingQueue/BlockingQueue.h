#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>

template <class  T>
class BlockingQueue {
    std::queue<T> m_queue;
    std::mutex m_mtx;
    std::condition_variable m_cond_var;
    const size_t m_max_size;
    bool m_done = false;
public:
    explicit BlockingQueue(size_t size):m_max_size(size) {
    }

    ~BlockingQueue()
    {
        m_cond_var.notify_all();
    }

    void push(const T& t)  {
        std::unique_lock<std::mutex> lock{m_mtx};
        m_cond_var.wait(lock,
                      [this] () { return m_done || m_queue.size() < m_max_size; }
                      );
        m_queue.push(t);
        lock.unlock();
        m_cond_var.notify_one();
    }

    void push(T&& t)  {
        std::unique_lock<std::mutex> lock{m_mtx};
        m_cond_var.wait(lock,
                      [this] () { return m_done || m_queue.size() < m_max_size; }
                      );
        m_queue.push(t);
        lock.unlock();
        m_cond_var.notify_one();
    }

    T pop() noexcept {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_cond_var.wait(lock, [this] { return !m_queue.empty(); });
        auto value = m_queue.front();
        m_queue.pop();
        lock.unlock();
        m_cond_var.notify_one();

        return value;
    }

    void try_push(const T& t)
    {
        std::unique_lock<std::mutex> lock{m_mtx};
        m_queue.push(t);
    }

    void try_push(T&& t)
    {
        std::unique_lock<std::mutex> lock{m_mtx};
        m_queue.push(t);
    }

    T try_pop()
    {
        std::unique_lock<std::mutex> lock{m_mtx};
        auto value = m_queue.front();
        m_queue.pop();
        return value;
    }

    size_t size() {
        std::unique_lock<std::mutex> lock{m_mtx};
        return m_queue.size();
    }

};
