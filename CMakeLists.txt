cmake_minimum_required(VERSION 3.13.4)
project(OpenCL)

enable_testing()
add_subdirectory(src)

if (WITH_TESTS)
    find_package(GTest CONFIG REQUIRED)
    message("Build binary file for UNIT tests ...")
    add_subdirectory(tests)
endif()

if (WITH_BENCHMARKS)
    message("Build binary file for benchmarks ...")
    add_subdirectory(benchs)
endif()