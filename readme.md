<!--
 * @Descripttion: readme.md
 * @version: 
 * @Author: liyu
 * @Date: 2022-06-05 23:52:23
-->
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

