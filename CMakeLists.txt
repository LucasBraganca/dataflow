cmake_minimum_required(VERSION 2.8)

project(main)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

include_directories("${CMAKE_SOURCE_DIR}/include")

file(GLOB_RECURSE H_SRCS ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE TEST_SRCS ${CMAKE_SOURCE_DIR}/test/*.cpp ${CMAKE_SOURCE_DIR}/test/*.h)

add_library(dataflow ${H_SRCS})
add_executable(main ${TEST_SRCS})
target_link_libraries(main dataflow)
set_target_properties(dataflow PROPERTIES LINKER_LANGUAGE CXX)
