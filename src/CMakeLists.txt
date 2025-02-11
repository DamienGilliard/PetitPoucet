cmake_minimum_required(VERSION 3.6)

project(PetitPoucet)

set(CMAKE_CXX_STANDARD 11)

execute_process(COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GIT_SUBMOD_RESULT
    )
if(NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
endif()

################################
# Flags
################################
option(BUILD_PETITPOUCET_EXECUTABLES "Build executables" ON)

################################
# Library
################################
add_subdirectory(PetitPoucet)

if(BUILD_PETITPOUCET_EXECUTABLES)
    add_subdirectory(executables)
endif()