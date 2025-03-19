cmake_minimum_required(VERSION 3.13.4)
project(OpenCL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()
add_subdirectory(src)

if (WITH_TESTS)
    find_package(GTest CONFIG REQUIRED)
    message("Build binary file for UNIT tests ...")
    add_subdirectory(tests)
endif()

if (WITH_BENCHMARKS)
    find_package(benchmark REQUIRED)
    message("Build binary file for benchmarks ...")
    add_subdirectory(benchs)
endif()