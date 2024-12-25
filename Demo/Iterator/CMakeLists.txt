add_executable(CircularIntIterator CircularIntIterator.cpp )
add_executable(FilterIntegerIterator FilterIntegerIterator.cpp )
add_executable(ReverseArrayIterator ReverseArrayIterator.cpp )

target_link_libraries(CircularIntIterator PRIVATE libutils)
target_link_libraries(FilterIntegerIterator PRIVATE libutils)
target_link_libraries(ReverseArrayIterator PRIVATE libutils)

target_include_directories(CircularIntIterator PRIVATE ../../Utils/)
target_include_directories(FilterIntegerIterator PRIVATE ../../Utils/)
target_include_directories(ReverseArrayIterator PRIVATE ../../Utils/)

