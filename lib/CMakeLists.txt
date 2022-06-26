#cmake版本
cmake_minimum_required(VERSION 3.0)

#设置编译时不输出警告
set(CMAKE_C_FLAGS   "-w -g")  

#debug时使用
set(CMAKE_BUILD_TYPE Debug)

#添加头文件目录
include_directories(../include/common ../include/def  employee_manager hash_table)

#工程名
file(GLOB em_source_list hash_table/*.c hash_table/*.h employee_manager/*.c employee_manager/*.h )

#添加核心算法的so库
add_library(employeemgr SHARED ${em_source_list})

#哈希表
file(GLOB hash_source_list hash_table/*.c hash_table/*.h employee_manager/*.c employee_manager/*.h )

#添加哈希表算法的so库
add_library(hashtable SHARED ${hash_source_list})