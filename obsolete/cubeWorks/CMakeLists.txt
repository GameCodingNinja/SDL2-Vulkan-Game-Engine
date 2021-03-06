# To build a release version on Linux, from within the cubeworks folder
# mkdir release
# cd release
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make

cmake_minimum_required(VERSION 3.0.1)

project(cubeworks)

# The version number.
set(gametemplate_VERSION_MAJOR 1)
set(gametemplate_VERSION_MINOR 0)

set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

# Add compile defines
#add_definitions(-D__ANDROID__)

# Check for C++11, -Wall = show warnings
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pthread")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Need to do this to find SDL2 libraries
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer)

# Use the normal means to find the other packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Boost COMPONENTS system filesystem thread REQUIRED)

# List all the include directories
include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2MIXER_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ../
    ../bulletPhysics/src
    ../library
    ../angelscript/include
    ../angelscript/add_on )

# List the directories in the order they are to be built
# This doesn't execute the build but more like cue's them up in thie below order
add_subdirectory( ../bulletPhysics ${CMAKE_CURRENT_BINARY_DIR}/bulletPhysics )
add_subdirectory( ../angelscript ${CMAKE_CURRENT_BINARY_DIR}/angelscript )
add_subdirectory( ../Box2D ${CMAKE_CURRENT_BINARY_DIR}/Box2D )
add_subdirectory( ../library ${CMAKE_CURRENT_BINARY_DIR}/library )
add_subdirectory( source )

