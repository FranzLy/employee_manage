cmake_minimum_required(VERSION 3.0)

project(EMPLOYEE_MANAGER)

set(CMAKE_C_FLAGS   "-w")  

set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

file(GLOB source_list src/*.c)

file(GLOB include_list include/*.h)

add_executable(employee_manager ${source_list} ${include_list})
