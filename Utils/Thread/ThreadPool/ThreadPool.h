#pragma once
#include <thread>
#include <vector>
#include <functional>
#include <future>

#include <Thread/BlockingQueue/BlockingQueue.h>

class ThreadPool {
    std::vector<std::thread> m_threads;
    size_t m_threads_num;
    BlockingQueue<std::function<void()>> m_tasks;
    std::atomic_bool m_isDone = false;
public:
    explicit ThreadPool(int task_count): m_threads_num(std::thread::hardware_concurrency()), m_tasks(task_count) {
        for (size_t i = 0; i < m_threads_num; ++i) {
            m_threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task = m_tasks.pop();
                    if (m_isDone) return;
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        m_isDone = true;

        for (int i = 0; i < m_threads_num; ++i) {
            m_tasks.push([]()
                {});
        }

        for (auto& thread : m_threads) {
            thread.join();
        }

    }

    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using R = typename std::invoke_result<F, Args...>::type;

        auto task = std::make_shared<std::packaged_task<R()>>(
            [f = std::forward<F>(f), args = std::make_tuple(std::forward<Args>(args)...)]() -> R {
                return std::apply(f, args);
            }
        );

        m_tasks.push([task]() { (*task)(); });
        return task->get_future();
    }

};
