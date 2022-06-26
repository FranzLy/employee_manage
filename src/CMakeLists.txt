#cmake版本
cmake_minimum_required(VERSION 3.0)

#工程名
project(EMPLOYEE_MANAGER)

#设置编译时不输出警告
set(CMAKE_C_FLAGS   "-w -g")  

#debug时使用
set(CMAKE_BUILD_TYPE Debug)

#添加头文件目录
include_directories(../include ../include/common ../include/def ../lib/employee_manager ../lib/hash_table command_parse)

#编译可执行文件所需文件
file(GLOB all_source_list execution/*.c command_parse/*.c ../lib/hash_table/*.c ../lib/employee_manager/*.c)
file(GLOB all_include_list ../include/common/*.h ../include/def/*.h ../lib/hash_table/*.h ../lib/employee_manager/*.h command_parse/*.h)

#添加可执行文件
add_executable(employee_manager ${all_source_list} ${all_include_list})

#编译动态库所需的文件
file(GLOB cli_source_list  command_parse/*.c ../lib/hash_table/*.c ../lib/employee_manager/*.c)
file(GLOB cli_include_list ../include/common/*.h ../include/def/*.h ../lib/hash_table/*.h ../lib/employee_manager/*.h command_parse/*.h)

#生成包含cli功能的so库
add_library(emmanager-cli SHARED ${cli_source_list} ${cli_include_list})