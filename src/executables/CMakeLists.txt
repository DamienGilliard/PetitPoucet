cmake_minimum_required(VERSION 3.10)
project(PetitPoucetExecutable)

set(SHARED_LIB_NAME petitpoucet)

# Add the executable source files
set(EXECUTABLE_NAME_1 correction_server)
set(EXECUTABLE_NAME_2 petitpoucet_executable)

add_executable(${EXECUTABLE_NAME_1}
            main2.cc)    
add_executable(${EXECUTABLE_NAME_2}
            main1.cc)       

target_link_libraries(${EXECUTABLE_NAME_1} ${SHARED_LIB_NAME}) 
target_link_libraries(${EXECUTABLE_NAME_2} ${SHARED_LIB_NAME})