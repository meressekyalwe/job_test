cmake_minimum_required(VERSION 3.0.0)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${MY_INCLUDE_DIR})
