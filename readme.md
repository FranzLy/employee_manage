<!--
 * @Descripttion: readme.md
 * @version: 
 * @Author: liyu
 * @Date: 2022-06-05 23:52:23
-->
# 目录结构
.
├── build                       ->本地编译目录
│   ├── CMakeCache.txt
│   ├── CMakeFiles
│   ├── cmake_install.cmake
│   ├── lib                     ->生成库
│   ├── Makefile
│   ├── src                     ->主程序可执行文件位置
│   └── test                    ->单元测试 覆盖率文档及内存检测可执行文件位置
├── CMakeLists.txt              ->主目录cmake文件
├── coverage-result.png         ->覆盖率截图
├── coverage-results.tar.gz     ->覆盖率文档压缩包
├── include                     ->头文件目录
│   ├── common                  ->常规头文件
│   └── def                     ->定义信息头文件
├── lib                         ->库
│   ├── CMakeLists.txt          ->库目录cmake文件
│   ├── employee_manager        ->员工信息管理代码
│   └── hash_table              ->哈希表代码
├── note.txt                    ->课程笔记
├── readme.md                   ->项目说明
├── scripts                     ->脚本目录
│   └── show_infos.txt          ->gdb显示信息脚本
├── src                         ->源文件目录
│   ├── CMakeLists.txt          ->源目录cmake文件
│   ├── command_parse           ->命令解析代码
│   └── execution               ->主函数代码
├── test                        ->测试目录
│   ├── CMakeLists.txt          ->测试目录cmake文件
│   ├── mem_check               ->内存检测代码
│   └── unit_test               ->单元测试代码
├── valgrind_report.log         ->内存检测报告
└── version                     ->版本说明

# 编译执行
    mkdir build && cd build
    cmake ..
    make
    cd src
    ./employee_manager

# 覆盖率查看
    cd build/test/unit_test/
    ./unit_test
    cd CMakeFiles/unit_test.dir
    lcov --directory . --capture --output-file app.info
    genhtml -o results app.info

# 内存检查
    cd build/test/mem_check
    valgrind --log-file=./valgrind_report.log --tool=memcheck -v --leak-check=full ./mem_check
    cat ./valgrind_report.log

