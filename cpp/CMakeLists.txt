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

file(GLOB_RECURSE RTKLIB_SOURCES ${RTKLIB_PATH}/src/rtklib.h 
    ${RTKLIB_PATH}/src/stream.c
    ${RTKLIB_PATH}/src/streamsvr.c
    ${RTKLIB_PATH}/src/rtkcmn.c
    ${RTKLIB_PATH}/src/trace.c
    ${RTKLIB_PATH}/src/solution.c
    ${RTKLIB_PATH}/src/sbas.c
    ${RTKLIB_PATH}/src/geoid.c
    ${RTKLIB_PATH}/src/rcvraw.c
    ${RTKLIB_PATH}/src/rcv/novatel.c
    ${RTKLIB_PATH}/src/rcv/ublox.c
    ${RTKLIB_PATH}/src/rcv/ss2.c
    ${RTKLIB_PATH}/src/rcv/crescent.c
    ${RTKLIB_PATH}/src/rcv/skytraq.c
    ${RTKLIB_PATH}/src/rcv/javad.c
    ${RTKLIB_PATH}/src/rcv/nvs.c
    ${RTKLIB_PATH}/src/rcv/binex.c
    ${RTKLIB_PATH}/src/rcv/rt17.c
    ${RTKLIB_PATH}/src/rtcm.c
    ${RTKLIB_PATH}/src/rtcm2.c
    ${RTKLIB_PATH}/src/rtcm3.c
    ${RTKLIB_PATH}/src/rtcm3e.c
    ${RTKLIB_PATH}/src/preceph.c
    ${RTKLIB_PATH}/src/rcv/septentrio.c
    ${RTKLIB_PATH}/src/rcv/swiftnav.c
    ${RTKLIB_PATH}/src/rcv/unicore.c)

add_library(rtk STATIC ${RTKLIB_SOURCES})
target_compile_options(rtk PRIVATE -fPIC)
target_compile_definitions(rtk PRIVATE ENAGLO ENAGAL ENAQZS ENACMP ENAIRN TRACE NFREQ=3 NEXOBS=3 SVR_REUSEADDR)
set_property(TARGET rtk PROPERTY C_STANDARD 99)
target_include_directories(rtk PUBLIC ${RTKLIB_PATH}/src)
set_target_properties(rtk PROPERTIES LINKER_LANGUAGE C)

############
# Executable
############

set(EXECUTABLE_NAME_1 main1)
set(EXECUTABLE_NAME_2 main2)
add_executable(${EXECUTABLE_NAME_1}
            main1.cc)
add_executable(${EXECUTABLE_NAME_2}
            main2.cc)

target_link_libraries(${EXECUTABLE_NAME_1} ${SHARED_LIB_NAME} rtk pthread) #RTKLIB depends on pthread
target_link_libraries(${EXECUTABLE_NAME_2} ${SHARED_LIB_NAME} rtk pthread) #RTKLIB depends on pthread

target_link_libraries(${SHARED_LIB_NAME} rtk)