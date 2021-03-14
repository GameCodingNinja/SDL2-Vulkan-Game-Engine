cmake_minimum_required(VERSION 3.10)

# set the project name
project(angelscript VERSION 2.84)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie -std=c++17 -Wall -pthread")

# Add the files to the library
add_library(
    ${PROJECT_NAME} STATIC
        source/as_atomic.cpp
        source/as_builder.cpp
        source/as_bytecode.cpp
        source/as_callfunc.cpp
        source/as_callfunc_mips.cpp
        source/as_callfunc_arm_gcc.S
        source/as_callfunc_arm.cpp
        source/as_callfunc_x86.cpp
        source/as_callfunc_x64_gcc.cpp
        source/as_callfunc_x64_msvc.cpp
        source/as_callfunc_x64_mingw.cpp
        source/as_compiler.cpp
        source/as_configgroup.cpp
        source/as_context.cpp
        source/as_datatype.cpp
        source/as_gc.cpp
        source/as_generic.cpp
        source/as_globalproperty.cpp
        source/as_memory.cpp
        source/as_module.cpp
        source/as_objecttype.cpp
        source/as_outputbuffer.cpp
        source/as_parser.cpp
        source/as_restore.cpp
        source/as_scriptcode.cpp
        source/as_scriptengine.cpp
        source/as_scriptfunction.cpp
        source/as_scriptnode.cpp
        source/as_scriptobject.cpp
        source/as_string.cpp
        source/as_string_util.cpp
        source/as_thread.cpp
        source/as_tokenizer.cpp
        source/as_typeinfo.cpp
        source/as_variablescope.cpp
        add_on/scriptstdstring/scriptstdstring.cpp
        add_on/scriptarray/scriptarray.cpp
        add_on/scriptdictionary/scriptdictionary.cpp
        add_on/scriptmath/scriptmath.cpp
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
        include
)

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm")
    enable_language(ASM)
endif()

