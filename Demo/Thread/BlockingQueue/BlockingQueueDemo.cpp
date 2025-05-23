#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#ifndef __APPLE__
#include <syncstream>
#endif


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
            #ifndef __APPLE__
            std::osyncstream(std::cout)
            #else
            std::cout
            #endif
            << "Producing " << std::endl;
            queue.push(10);

            #ifndef __APPLE__
            std::osyncstream(std::cout)
            #else
            std::cout
            #endif
            << "Buffer size after producing: " << queue.size() << std::endl << std::endl;
        }
    });

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);
    std::thread consumerThread([&] {
        for (int i = 1; i <= 20; ++i) {
            int value = queue.pop();
            #ifndef __APPLE__
            std::osyncstream(std::cout)
            #else
            std::cout
            #endif
            << "Consuming " << value << std::endl;
            #ifndef __APPLE__
            std::osyncstream(std::cout)
            #else
            std::cout
            #endif
            << "Buffer size after consuming: " << queue.size() << std::endl << std::endl;
        }
    });
    producerThread.join();
    consumerThread.join();
    return 0;
}


int main() {
    test1();
}