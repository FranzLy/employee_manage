#cmake版本
cmake_minimum_required(VERSION 3.0)

#工程名
project(EMPLOYEE_MANAGER)

#设置编译时不输出警告
set(CMAKE_C_FLAGS   "-w")  

#debug时使用
set(CMAKE_BUILD_TYPE Debug)

#指定so库输出位置
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

#添加头文件目录
include_directories(include)

#编译so库所需文件
file(GLOB core_source_list src/hash_data.c src/employee_utils.c src/employee_mgr.c )
file(GLOB core_include_list include/common/*.h include/hash_data.h include/employee*.h)

#编译可执行文件所需文件
file(GLOB all_source_list src/*.c)
file(GLOB all_include_list include/*.h)

#添加可执行文件
add_executable(employee_manager ${all_include_list} ${all_source_list} )

#添加so库
add_library(employeemgr SHARED ${core_include_list} ${core_source_list})
