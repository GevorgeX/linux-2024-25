add_executable(BlockingQueueDemo BlockingQueue/BlockingQueueDemo.cpp)
target_include_directories(BlockingQueueDemo PRIVATE ../../Utils)
target_link_libraries(BlockingQueueDemo PRIVATE libutils)