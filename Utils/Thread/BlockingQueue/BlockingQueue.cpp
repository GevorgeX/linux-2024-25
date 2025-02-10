#include "BlockingQueue.h"

template<class T>
BlockingQueue<T>::BlockingQueue(int size):max_size(size) {
}

template<class T>
void BlockingQueue<T>::push(const T &t) {
    std::unique_lock<std::mutex> lock{mtx};
    cond_var.wait(lock,
                  [this] () { return queue.size() < max_size; }
                  );
    queue.push(t);
    lock.unlock();
    cond_var.notify_one();
}
template<class T>
T BlockingQueue<T>::pop() {
    std::unique_lock<std::mutex> lock(mtx);
    cond_var.wait(lock, [this] { return !queue.empty(); });
    auto value = queue.front();
    queue.pop();
    lock.unlock();
    cond_var.notify_one();

    return value;
}

template<class T>
int BlockingQueue<T>::size() const {
    return queue.size();
}

template class BlockingQueue<int>;