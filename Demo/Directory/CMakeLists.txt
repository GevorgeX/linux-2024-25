add_executable(Directory DirectoryDemo.cpp)
target_include_directories(Directory PRIVATE ../../Utils/)
target_link_libraries(Directory PRIVATE libutils)

add_custom_command(TARGET Directory POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/TEST $<TARGET_FILE_DIR:Directory>/TEST)