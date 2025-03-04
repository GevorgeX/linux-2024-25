add_executable(BlockingQueueDemo BlockingQueue/BlockingQueueDemo.cpp)
target_include_directories(BlockingQueueDemo PRIVATE ../../Utils)

add_executable(ThreadPoolDemo ThreadPool/ThreadPoolDemo.cpp)
target_include_directories(ThreadPoolDemo PRIVATE ../../Utils)