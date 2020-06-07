# To build a release version on Linux, from within the gameTemplate folder
# mkdir release
# cd release
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make

cmake_minimum_required(VERSION 3.0.1)

project(consoleSimpleApp)

# The version number.
set(consoleSimpleApp_VERSION_MAJOR 1)
set(consoleSimpleApp_VERSION_MINOR 0)

# Check for C++20, -Wall = show warnings; -Wextra = more warnings
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++20" COMPILER_SUPPORTS_CXX20)
if(COMPILER_SUPPORTS_CXX20)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie -std=c++20 -Wall -pthread")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++20 support. Please use a different C++ compiler.")
endif()

# List all the include directories
include_directories(
    ${Boost_INCLUDE_DIRS}
    ../ )

# List the directories in the order they are to be built
# This doesn't execute the build but more like queue's them up in the below order
add_subdirectory( consoleSimpleApp )
