# To build a release version on Linux, from within this project folder
# mkdir release
# cd release
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make

cmake_minimum_required(VERSION 3.5)
include(ExternalProject)

project(DesignerApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the normal means to find the other packages
find_package(Vulkan REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

# Add the game level executable files
add_executable(
    ${PROJECT_NAME}
    source/main.cpp
    source/mainwindow.cpp
    source/mainwindow.h
    source/mainwindow.ui
)

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

# Append external libraries and includes
list(APPEND EXTRA_LIBS ${bulletPhysics_SOURCE_DIR}/build/${CMAKE_STATIC_LIBRARY_PREFIX}bulletPhysics${CMAKE_STATIC_LIBRARY_SUFFIX})
list(APPEND EXTRA_LIBS ${Box2D_SOURCE_DIR}/build/${CMAKE_STATIC_LIBRARY_PREFIX}Box2D${CMAKE_STATIC_LIBRARY_SUFFIX})
list(APPEND EXTRA_LIBS ${angelscript_SOURCE_DIR}/build/${CMAKE_STATIC_LIBRARY_PREFIX}angelscript${CMAKE_STATIC_LIBRARY_SUFFIX})
list(APPEND EXTRA_LIBS ${library_SOURCE_DIR}/build/${CMAKE_STATIC_LIBRARY_PREFIX}library${CMAKE_STATIC_LIBRARY_SUFFIX})
list(APPEND EXTRA_LIBS /usr/lib/${CMAKE_SHARED_LIBRARY_PREFIX}SDL2${CMAKE_SHARED_LIBRARY_SUFFIX})
list(APPEND EXTRA_LIBS /usr/lib/${CMAKE_SHARED_LIBRARY_PREFIX}SDL2_mixer${CMAKE_SHARED_LIBRARY_SUFFIX})
list(APPEND EXTRA_LIBS Qt${QT_VERSION_MAJOR}::Widgets)
list(APPEND EXTRA_INCLUDES /usr/include/SDL2)
list(APPEND EXTRA_INCLUDES ${Boost_INCLUDE_DIRS})

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
