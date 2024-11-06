cmake_minimum_required(VERSION 3.6)

project(Dataforging)

set(CMAKE_CXX_STANDARD 11)

execute_process(COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GIT_SUBMOD_RESULT
    )
if(NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
endif()

################################
# Dynamic library of PetitPoucet
################################

set(SHARED_LIB_NAME petitpoucet)

file(GLOB_RECURSE SOURCES_LIB
    PetitPoucet.hh
    PetitPoucet/*.cc)

add_library(${SHARED_LIB_NAME} SHARED ${SOURCES_LIB})

target_include_directories(${SHARED_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PetitPoucet)   
###################################
# 3rd party libraries paths
###################################
# RTKLIB
set(RTKLIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/deps/RTKLIB)
include_directories(${RTKLIB_PATH}/src)

############
# Executable
############

set(EXECUTABLE_NAME main)
add_executable(${EXECUTABLE_NAME}
            main.cc)

target_link_libraries(${EXECUTABLE_NAME} ${SHARED_LIB_NAME})