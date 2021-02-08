cmake_minimum_required(VERSION 3.10)

# set the project name
project(library VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(
    ${PROJECT_NAME} STATIC
        library.cpp
) 
