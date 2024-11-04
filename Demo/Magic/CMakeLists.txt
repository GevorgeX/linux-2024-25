add_executable(DoMagic do_magic.cpp DoMagicTest.cpp)
add_executable(DoMagic2 DoMagicTest_2.cpp)

add_custom_command(TARGET DoMagic POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/TestFiles $<TARGET_FILE_DIR:DoMagic>/TestFiles)
add_custom_command(TARGET DoMagic2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/TestFiles $<TARGET_FILE_DIR:DoMagic2>/TestFiles)