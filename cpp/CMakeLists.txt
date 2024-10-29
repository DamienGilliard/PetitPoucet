cmake_minimum_required(VERSION 3.6)

project(Dataforging)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(../../rtklib/RTKLIB-b34k/src)

add_executable(main
            main.cc)