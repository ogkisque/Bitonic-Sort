set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

find_package(OpenCL REQUIRED)

add_library(cl_lib INTERFACE)
target_compile_features(cl_lib INTERFACE cxx_std_20)
target_include_directories(cl_lib INTERFACE ${INCLUDE_DIR})
target_include_directories(cl_lib INTERFACE ${OpenCL_INCLUDE_DIRS})
target_link_libraries(cl_lib INTERFACE ${OpenCL_LIBRARIES})

add_executable(main main.cpp)
target_compile_features(main PUBLIC cxx_std_20)
target_link_libraries(main cl_lib)
