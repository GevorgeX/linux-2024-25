cmake_minimum_required(VERSION 3.16)
project(linux_homeworks)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Utils)
add_subdirectory(Demo/Iterator)
add_subdirectory(Demo/Directory)
add_subdirectory(Demo/Magic)
add_subdirectory(Demo/SuperRm)
add_subdirectory(Demo/ArgumentParser)
add_subdirectory(Demo/Cp)
add_subdirectory(Demo/SerDes)
if (NOT APPLE)
add_subdirectory(Demo/DirectoryWatcher)
endif ()
add_subdirectory(Demo/Allocator)
add_subdirectory(Demo/Thread)
add_subdirectory(Demo/Ussr)
