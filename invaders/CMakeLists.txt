# To build a release version on Linux, from within this project folder
# mkdir release
# cd release
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make

cmake_minimum_required(VERSION 3.10)
include(ExternalProject)

project(invaders VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie -std=c++17 -Wall -pthread")

# Use the normal means to find the other packages
find_package(Vulkan REQUIRED)

# Create library specific path variables
get_filename_component(PARENT_SOURCE_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
set(bulletPhysics_SOURCE_DIR ${PARENT_SOURCE_DIR}/bulletPhysics)
set(Box2D_SOURCE_DIR ${PARENT_SOURCE_DIR}/Box2D)
set(angelscript_SOURCE_DIR ${PARENT_SOURCE_DIR}/angelscript)
set(library_SOURCE_DIR ${PARENT_SOURCE_DIR}/library)

# ExternalProject_Add cmake files are generated during the build command

# Add the external project library.
ExternalProject_Add(
    library
    SOURCE_DIR ${library_SOURCE_DIR}
    BINARY_DIR ${library_SOURCE_DIR}/build
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "Skipping install step."
    BUILD_ALWAYS 1
)

# Add the external project Bullet Physics.
ExternalProject_Add(
    bulletPhysics
    SOURCE_DIR ${bulletPhysics_SOURCE_DIR}
    BINARY_DIR ${bulletPhysics_SOURCE_DIR}/build
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "Skipping install step."
)

# Add the external project Box2D.
ExternalProject_Add(
    Box2D
    SOURCE_DIR ${Box2D_SOURCE_DIR}
    BINARY_DIR ${Box2D_SOURCE_DIR}/build
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "Skipping install step."
)

# Add the external project angelscript.
ExternalProject_Add(
    angelscript
    SOURCE_DIR ${angelscript_SOURCE_DIR}
    BINARY_DIR ${angelscript_SOURCE_DIR}/build
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "Skipping install step."
)

# Add the game level executable files
add_executable(
    ${PROJECT_NAME}
        source/game/game.cpp
        source/game/main.cpp
        source/state/commonstate.cpp
        source/state/startupstate.cpp
        source/state/titlescreenstate.cpp
        source/state/loadstate.cpp
        source/state/level1state.cpp
)

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^aarch64")
    set(SDL_LIB_DIR /usr/lib/aarch64-linux-gnu/)
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm")
    set(SDL_LIB_DIR /usr/lib/arm-linux-gnueabihf/)
else()
    set(SDL_LIB_DIR /usr/lib/)
endif()

# Append external libraries and includes. Libs need to be listed in order of dependency for some reason
list(APPEND EXTRA_LIBS ${library_SOURCE_DIR}/build/${CMAKE_STATIC_LIBRARY_PREFIX}library${CMAKE_STATIC_LIBRARY_SUFFIX})
list(APPEND EXTRA_LIBS ${bulletPhysics_SOURCE_DIR}/build/${CMAKE_STATIC_LIBRARY_PREFIX}bulletPhysics${CMAKE_STATIC_LIBRARY_SUFFIX})
list(APPEND EXTRA_LIBS ${Box2D_SOURCE_DIR}/build/${CMAKE_STATIC_LIBRARY_PREFIX}Box2D${CMAKE_STATIC_LIBRARY_SUFFIX})
list(APPEND EXTRA_LIBS ${angelscript_SOURCE_DIR}/build/${CMAKE_STATIC_LIBRARY_PREFIX}angelscript${CMAKE_STATIC_LIBRARY_SUFFIX})
list(APPEND EXTRA_LIBS ${Vulkan_LIBRARY})
list(APPEND EXTRA_LIBS ${SDL_LIB_DIR}${CMAKE_SHARED_LIBRARY_PREFIX}SDL3${CMAKE_SHARED_LIBRARY_SUFFIX})
list(APPEND EXTRA_LIBS ${SDL_LIB_DIR}${CMAKE_SHARED_LIBRARY_PREFIX}SDL2_mixer${CMAKE_SHARED_LIBRARY_SUFFIX})
list(APPEND EXTRA_INCLUDES /usr/include/SDL3)
list(APPEND EXTRA_INCLUDES ${PARENT_SOURCE_DIR})
list(APPEND EXTRA_INCLUDES ${Boost_INCLUDE_DIRS})
list(APPEND EXTRA_INCLUDES ${library_SOURCE_DIR})
list(APPEND EXTRA_INCLUDES ${angelscript_SOURCE_DIR}/include)
list(APPEND EXTRA_INCLUDES ${angelscript_SOURCE_DIR}/add_on)
list(APPEND EXTRA_INCLUDES ${bulletPhysics_SOURCE_DIR}/src)

# Target all the libraries
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
        ${EXTRA_LIBS}
)

# Target all then includes
target_include_directories(
    ${PROJECT_NAME} PRIVATE
        ${EXTRA_INCLUDES}
)

message(STATUS "Extra Libs ${EXTRA_LIBS}")
message(STATUS "Extra Inc ${EXTRA_INCLUDES}")
