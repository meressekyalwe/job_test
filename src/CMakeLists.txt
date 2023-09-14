cmake_minimum_required(VERSION 3.0.0)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(job_test main.cpp)
target_include_directories(job_test PUBLIC ${MY_INCLUDE_DIR})