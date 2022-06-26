#cmake版本
cmake_minimum_required(VERSION 3.0)

#指定编译器
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

#工程名
project(unit_test)

#设置编译时不输出警告,采用C++11标准编译
set(CMAKE_C_FLAGS   "-w -g -fprofile-arcs -ftest-coverage") 
set(CMAKE_CXX_FLAGS   "-w -g -fprofile-arcs -ftest-coverage") 
set(CMAKE_CXX_STANDARD 11)

#debug时使用
set(CMAKE_BUILD_TYPE Debug)

#添加头文件目录
include_directories(/usr/local/include/ ../../include ../../include/common ../../include/def ../../lib/employee_manager ../../lib/hash_table ../../src/command_parse)

#编译可执行文件所需文件
file(GLOB all_source_list ./*.cpp ../../src/command_parse/*.c ../../lib/hash_table/*.c ../../lib/employee_manager/*.c)
file(GLOB all_include_list ../../include/common/*.h ../../include/def/*.h ../../lib/hash_table/*.h ../../lib/employee_manager/*.h ../../src/command_parse/*.h)

#添加可执行文件
add_executable(unit_test ${all_source_list} ${all_include_list})

#link_directories(${PROJECT_SOURCE_DIR}/lib) #添加动态连接库的路径
target_link_libraries(unit_test -lgtest -lpthread) #添加libgtest lpthread