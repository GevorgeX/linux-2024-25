#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <syncstream>

#include "Thread/BlockingQueue/BlockingQueue.h"

void producer(std::mutex& mtx, std::condition_variable& cond_var,
              std::queue<int>& buffer, const size_t MAX_BUFFER_SIZE) {
    std::unique_lock<std::mutex> lock{mtx};

    cond_var.wait(lock,
                  [&buffer, MAX_BUFFER_SIZE] () { return buffer.size() < MAX_BUFFER_SIZE; }
                  );
    std::cout << "Producing " << std::endl;
    buffer.push(10);
    std::cout << "Buffer size after producing: " << buffer.size() << std::endl << std::endl;
    lock.unlock();
    cond_var.notify_one();
}

void consumer(std::mutex& mtx, std::condition_variable& cond_var,
              std::queue<int>& buffer, const size_t MAX_BUFFER_SIZE) {
    std::unique_lock<std::mutex> lock(mtx);
    cond_var.wait(lock, [&buffer] { return !buffer.empty(); });
    int value = buffer.front();
    buffer.pop();
    std::cout << "Consuming " << value << std::endl;
    std::cout << "Buffer size after consuming: " << buffer.size() << std::endl << std::endl;
    lock.unlock();
    cond_var.notify_one();
}

int test1() {
    std::mutex mtx;
    std::condition_variable cond_var;
    std::queue<int> buffer;
    constexpr size_t MAX_BUFFER_SIZE = 10;
    std::thread producerThread([&, MAX_BUFFER_SIZE] {
        for (int i = 1; i <= 20; ++i) {
            producer(mtx, cond_var, buffer, MAX_BUFFER_SIZE);
        }
    });

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);
    std::thread consumerThread([&, MAX_BUFFER_SIZE] {
        for (int i = 1; i <= 20; ++i) {
            consumer(mtx, cond_var, buffer, MAX_BUFFER_SIZE);
        } // Ov karda, halal a
    });
    producerThread.join();
    consumerThread.join();
    return 0;
}

int test2() {

    BlockingQueue<int> queue(10);

    std::thread producerThread([&] {
        for (int i = 1; i <= 20; ++i) {
            std::osyncstream(std::cout) << "Producing " << std::endl;
            queue.push(10);
            std::osyncstream(std::cout) << "Buffer size after producing: " << queue.size() << std::endl << std::endl;
        }
    });

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);
    std::thread consumerThread([&] {
        for (int i = 1; i <= 20; ++i) {
            int value = queue.pop();
            std::osyncstream(std::cout) << "Consuming " << value << std::endl;
            std::osyncstream(std::cout) << "Buffer size after consuming: " << queue.size() << std::endl << std::endl;
        }
    });
    producerThread.join();
    consumerThread.join();
    return 0;
}


int main() {
    // test1();
    test2();
}