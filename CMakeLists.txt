cmake_minimum_required(VERSION 3.13)
project(avt LANGUAGES CXX)
set(CMAKE_CXX_CLANG_TIDY clang-tidy-11)
add_compile_options(-Wall -Wextra -Werror)
add_executable(avt avt.h main.cpp)
include(CTest)
add_test(main main)
if(NOT REPORT_PATH)
    set(REPORT_PATH ${PROJECT_BINARY_DIR})
endif(NOT REPORT_PATH)
add_custom_target(coverage
    COMMAND find "'('" -name *.cpp.gcno "')'" -exec gcov "{}" "+" > /dev/null
    COMMAND lcov -q -o tests.info -t "tests" -c -d . --rc lcov_branch_coverage=1 --exclude '/usr/include/*'
    COMMAND genhtml -q -o ${REPORT_PATH}/coverage_report tests.info --rc lcov_branch_coverage=1
)