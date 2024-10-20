add_executable(CircularIntIterator CircularIntIterator.cpp )
add_executable(FilterIntegerIterator FilterIntegerIterator.cpp )
add_executable(ReverseArrayIterator ReverseArrayIterator.cpp )
add_executable(argument ArgumentParser.cpp)

target_include_directories(CircularIntIterator PRIVATE ../../Utils/)
target_include_directories(FilterIntegerIterator PRIVATE ../../Utils/)
target_include_directories(ReverseArrayIterator PRIVATE ../../Utils/)

