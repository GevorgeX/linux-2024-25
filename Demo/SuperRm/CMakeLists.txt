add_executable(SuperRm SuperRmDemo.cpp)
target_include_directories(SuperRm PRIVATE ../../Utils)

add_custom_command(TARGET SuperRm POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/../Directory/TEST $<TARGET_FILE_DIR:Directory>/../SuperRm/TEST)