set(SOURCES Iterator/CircularIntIterator.cpp
        Iterator/FilterIntegerIterator.cpp
        Iterator/ReverseArrayIterator.cpp
        Allocator/Allocator.cpp
        Directory/Directory.cpp
        SuperRm/SuperRm.cpp
        ArgumentParser/ArgumentParser.cpp
        Cp/Cp.cpp
        SerDes/SerDes.cpp
        DoCommand/DoCommand.cpp
        Xargs/Xargs.cpp
)

if(NOT APPLE)
    list(APPEND SOURCES DirectoryWatcher/DirectoryWatcher.cpp)
endif ()

    add_library(libutils ${SOURCES})
