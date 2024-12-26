add_executable(CpDemo CpDemo.cpp)
target_include_directories(CpDemo PRIVATE ../../Utils/)
target_link_libraries(CpDemo PRIVATE libutils)
