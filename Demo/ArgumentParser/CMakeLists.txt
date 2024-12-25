add_executable(ArgumentParser ArgumentParserDemo.cpp)
target_include_directories(ArgumentParser PRIVATE ../../Utils/)
target_link_libraries(ArgumentParser PRIVATE libutils)