set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

find_package(OpenCL 2.0 REQUIRED)

add_library(cl_lib INTERFACE)
target_include_directories(cl_lib INTERFACE ${INCLUDE_DIR})
target_link_libraries(cl_lib INTERFACE OpenCL::OpenCL)

add_executable(main main.cpp)
target_link_libraries(main cl_lib)