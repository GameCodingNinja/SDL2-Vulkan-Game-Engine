# To build a release version on Linux, from within the gameTemplate folder
# mkdir release
# cd release
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make

cmake_minimum_required(VERSION 3.0.1)

project(consoleTestAp)

# The version number.
set(consoleTestAp_VERSION_MAJOR 1)
set(consoleTestAp_VERSION_MINOR 0)

# Check for C++17, -Wall = show warnings; -Wextra = more warnings
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
if(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie -std=c++17 -Wall -pthread")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
endif()

set(SDL2_INCLUDE_DIRS /usr/include/SDL2)
set(SDL2_LIBRARY /usr/lib/libSDL2.so)
set(SDL2MIXER_LIBRARY /usr/lib/libSDL2_mixer.so)

# Use the normal means to find the other packages
find_package(Vulkan REQUIRED)

# List all the include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2MIXER_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ../
    ../bulletPhysics/src
    ../library
    ../angelscript/include
    ../angelscript/add_on )

# List the directories in the order they are to be built
# This doesn't execute the build but more like queue's them up in the below order
add_subdirectory( ../bulletPhysics ${CMAKE_CURRENT_BINARY_DIR}/bulletPhysics )
add_subdirectory( ../angelscript ${CMAKE_CURRENT_BINARY_DIR}/angelscript )
add_subdirectory( ../Box2D ${CMAKE_CURRENT_BINARY_DIR}/Box2D )
add_subdirectory( ../library ${CMAKE_CURRENT_BINARY_DIR}/library )
add_subdirectory( consoleTestAp )

