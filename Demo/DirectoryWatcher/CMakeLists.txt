set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

add_executable(DirectoryWatcherTest DirectoryWatcherTest.cpp)
target_include_directories(DirectoryWatcherTest PRIVATE ../../Utils/)
target_link_libraries(DirectoryWatcherTest PRIVATE libutils)