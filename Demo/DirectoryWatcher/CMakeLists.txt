add_executable(DirectoryWatcherTest DirectoryWatcherTest.cpp)
target_include_directories(DirectoryWatcherTest PRIVATE ../../Utils/)

add_custom_command(TARGET DirectoryWatcherTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/TEST $<TARGET_FILE_DIR:DirectoryWatcherTest>/TEST)