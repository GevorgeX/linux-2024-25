#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <syncstream>

#include "Thread/BlockingQueue/BlockingQueue.h"

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