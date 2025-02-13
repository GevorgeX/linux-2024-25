#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <syncstream>

#include "Thread/BlockingQueue/BlockingQueue.h"

int test1() {
    BlockingQueue<int>* queue = new BlockingQueue<int>(1);

    queue->push(1);

    std::thread prod([](BlockingQueue<int>* q) {
    std::cout << "Producer started, trying to push..." << std::endl;
    q->push(42);
    std::cout << "Producer finished push!" << std::endl;
        }, queue);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Main thread: Destroying queue!" << std::endl;
    delete queue;

    prod.join();
    std::cout << "Main thread: Finished!" << std::endl;
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
    test1();
}