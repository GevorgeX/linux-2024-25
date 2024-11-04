project(iterator)

add_executable(CircularIntIterator CircularIntIterator.cpp )
add_executable(FilterIntegerIterator FilterIntegerIterator.cpp )
add_executable(ReverseArrayIterator ReverseArrayIterator.cpp )
add_executable(Directory Directory.cpp)

include_directories(../../Utils/)

add_custom_command(TARGET Directory POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/TEST $<TARGET_FILE_DIR:Directory>/TEST)