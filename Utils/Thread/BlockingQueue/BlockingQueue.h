#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>

template <class  T>
class BlockingQueue {
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cond_var;
    int max_size;
public:
    explicit BlockingQueue(int size);
    void push(const T& t);
    T pop();
    int size() const;
};
