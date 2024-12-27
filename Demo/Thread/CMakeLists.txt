add_executable(ThreadAPITest main.cpp)
target_include_directories(ThreadAPITest PRIVATE ../../Utils)
#target_link_libraries(SuperRm PRIVATE libutils)
target_link_libraries(ThreadAPITest PUBLIC -lpthread)