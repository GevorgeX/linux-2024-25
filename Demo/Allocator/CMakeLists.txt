add_executable(AllocatorDemo AllocatorDemo.cpp)
target_include_directories(AllocatorDemo PRIVATE ../../Utils/)
target_link_libraries(AllocatorDemo PRIVATE libutils)