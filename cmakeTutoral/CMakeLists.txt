cmake_minimum_required(VERSION 3.10)
include(ExternalProject)

# set the project name
project(Tutorial VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(${PROJECT_NAME} source/Tutorial.cpp)

# Get the parent directory
get_filename_component(PARENT_SOURCE_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)

# Add the external project. The make files are built during the build command
set(library_SOURCE_DIR ${PARENT_SOURCE_DIR}/cmakeTutoralLib)
ExternalProject_Add(
    library
    SOURCE_DIR ${library_SOURCE_DIR}
    BINARY_DIR ${library_SOURCE_DIR}/build
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "Skipping install step."
)

# Append extra libraries and includes
list(APPEND EXTRA_LIBS ${library_SOURCE_DIR}/build/${CMAKE_STATIC_LIBRARY_PREFIX}library${CMAKE_STATIC_LIBRARY_SUFFIX})
list(APPEND EXTRA_INCLUDES ${library_SOURCE_DIR})

# List all the libraries to link against
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

# add the binary tree to the search path for include files
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_BINARY_DIR}
    ${PROJECT_BINARY_DIR}/source
    ${EXTRA_INCLUDES}
)

message(STATUS "Parent Dir: ${PARENT_SOURCE_DIR}")
message(STATUS "Binary Dir: ${PROJECT_BINARY_DIR}")
message(STATUS "Source Dir: ${PROJECT_SOURCE_DIR}")
message(STATUS "Current Source Dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Extra Libs: ${EXTRA_LIBS}")
message(STATUS "Extra Includes: ${EXTRA_INCLUDES}")