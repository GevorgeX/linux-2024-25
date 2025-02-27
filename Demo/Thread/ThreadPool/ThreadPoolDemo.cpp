#include <iostream>
#include <cassert>

#include <Thread/ThreadPool/ThreadPool.h>
void test_threadpool() {
    ThreadPool pool(4);

    // Test 1: Simple addition
    auto future1 = pool.enqueue([](int a, int b) { return a + b; }, 10, 20);
    assert(future1.get() == 30);
    std::cout << "Test 1 passed: Addition works correctly." << std::endl;

    // Test 2: Multiple tasks with different return types
    auto future2 = pool.enqueue([] { return std::string("Hello from thread!"); });
    assert(future2.get() == "Hello from thread!");
    std::cout << "Test 2 passed: String return works correctly." << std::endl;

    // Test 3: Task with delay
    auto future3 = pool.enqueue([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return 42;
    });
    assert(future3.get() == 42);
    std::cout << "Test 3 passed: Delayed execution works correctly." << std::endl;

    // Test 4: Run multiple tasks and check parallelism
    std::vector<std::future<int>> futures;
    for (int i = 0; i < 10; i++) {
        futures.push_back(pool.enqueue([i] { return i * i; }));
    }

    for (int i = 0; i < 10; i++) {
        assert(futures[i].get() == i * i);
    }
    std::cout << "Test 4 passed: Multiple parallel tasks executed correctly." << std::endl;

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    test_threadpool();
    return 0;
}
/*int main() {
    ThreadPool pool(10);

    for (int i = 0; i < 5; ++i) {
        pool.enqueue([i] {
            std::cout << "Task " << i << " is running on thread "
                 << std::this_thread::get_id() << std::endl;
            // Simulate some work
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout<<"END";
}*/