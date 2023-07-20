cmake_minimum_required(VERSION 3.13)
project(avt LANGUAGES CXX)

enable_testing()
find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)
set(COVERAGE_REPORT_DIR "${CMAKE_BINARY_DIR}/coverage")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

add_compile_options(-Wall -Wextra -Werror)

add_executable(avt avt.h main.cpp)

include(CTest)
add_test(NAME main COMMAND avt) 

add_custom_target(coverage
    COMMAND "${LCOV_PATH}" --capture --directory . --output-file coverage.info
    COMMAND "${GENHTML_PATH}" -o "${COVERAGE_REPORT_DIR}" coverage.info
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Generating code coverage report"
)

add_custom_target(coverage-report
    COMMAND "${CMAKE_MAKE_PROGRAM}" coverage
    COMMAND "${GENHTML_PATH}" -o "${COVERAGE_REPORT_DIR}" coverage.info
    COMMENT "Generating and displaying code coverage report"
)
