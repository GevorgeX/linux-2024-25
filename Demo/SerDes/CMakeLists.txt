add_executable(SerDes SerDesDemo.cpp )
target_include_directories(SerDes PRIVATE ../../Utils/)
target_link_libraries(SerDes PRIVATE libutils)
