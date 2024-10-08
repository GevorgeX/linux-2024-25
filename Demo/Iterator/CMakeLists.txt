project(iterator)


add_executable(iterator CircularIntIterator.cpp )
add_executable(iterator FilterIntegerIterator.cpp )
add_executable(iterator ReverseArrayIterator.cpp )

target_include_directories(iterator PRIVATE ../../Utils/)
