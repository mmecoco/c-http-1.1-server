cmake_minimum_required(VERSION 3.14)
project(CPP_zia_2019)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE off) # Turn on to debug cmake / make

set(ZIA_PTHREAD "")
set(ZIA_WWW_DIR "${CMAKE_BINARY_DIR}/www")
set(ZIA_CONFIG_DIR "${CMAKE_BINARY_DIR}/ConfigDir")
set(ZIA_MODULE_DIR "${CMAKE_BINARY_DIR}/ModuleDir")
set(ZIA_UTILS utils)
set(ZIA_UTILS_INCLUDE Utils/include)

# Include directly the openZia.cmake to your CMake build
# This include gives 'openZiaLibs' and 'openZiaIncludes' variables.
include(./openZia/openZia/openZia.cmake)

if (UNIX)
    set(ZIA_PTHREAD "pthread")
    add_compile_options(-Wall -Wextra -Weffc++)

    if (CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_definitions(DEBUG=1)
        add_compile_options(-O0 -g3)
    elseif (CMAKE_BUILD_TYPE MATCHES Release)
        add_compile_definitions(RELEASE=1)
        add_compile_options(-O3 -g0)
    endif ()
endif ()

message(STATUS "Loading conan...")
set(CONAN_CMAKE_SILENT_OUTPUT TRUE)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

link_libraries(${openZiaLibs} ${ZIA_UTILS} ${ZIA_PTHREAD} ${CONAN_LIBS})
include_directories(${CMAKE_INCLUDE_PATH} ${openZiaIncludes} ${ZIA_UTILS_INCLUDE})

# Setup runtime environment
file(MAKE_DIRECTORY ${ZIA_WWW_DIR})
file(MAKE_DIRECTORY ${ZIA_CONFIG_DIR})
file(MAKE_DIRECTORY ${ZIA_MODULE_DIR})
file(COPY Pages/favicon.png DESTINATION ${ZIA_WWW_DIR})

# Core
add_subdirectory(Utils)
add_subdirectory(Server)
add_subdirectory(Modules/Parser)
add_subdirectory(Modules/Config)

# Methods
add_subdirectory(Modules/Get)
add_subdirectory(Modules/Head)
add_subdirectory(Modules/Delete)
add_subdirectory(Modules/Option)
add_subdirectory(Modules/Put)

# Other
add_subdirectory(Modules/Index)
add_subdirectory(Modules/Error)
add_subdirectory(Modules/PhpScript)
add_subdirectory(Modules/SSLModule)

add_dependencies(server openZia utils get head option delete config parser index php_script sslmodule error)
