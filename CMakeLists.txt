#cmake版本
cmake_minimum_required(VERSION 3.0)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#debug时使用
set(CMAKE_BUILD_TYPE Debug)

#添加so库
add_subdirectory(lib)

#编译可执行文件
add_subdirectory(src)

#编译单元测试
add_subdirectory(test)

