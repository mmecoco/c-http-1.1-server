cmake_minimum_required(VERSION 3.14)
project(php_script)

set(CMAKE_CXX_STANDARD 17)

add_library(
        ${PROJECT_NAME} SHARED
        include/PhpScript.hpp src/PhpScript.cpp
        include/ExeCommand.hpp src/ExeCommand.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${ZIA_MODULE_DIR}
)
