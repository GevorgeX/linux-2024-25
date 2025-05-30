add_executable(Client client.cpp)
add_executable(Server server.cpp)
target_include_directories(Server PRIVATE ../Utils/)
