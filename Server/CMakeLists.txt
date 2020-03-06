cmake_minimum_required(VERSION 3.14)
project(server)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        ${PROJECT_NAME}
        src/Main.cpp
        src/HttpServer.cpp include/HttpServer.hpp
        src/Socket.cpp include/Socket.hpp
        src/ServerConfig.cpp include/ServerConfig.hpp
        include/code_to_string.hpp
        src/Signal.cpp include/Signal.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)
