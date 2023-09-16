cmake_minimum_required(VERSION 3.0.0)

project(job_test) 

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${MY_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
add_definitions(-DRESOURCE_DIR="${CMAKE_SOURCE_DIR}/resource/")
add_definitions(-DOUTPUT_DIR="${CMAKE_SOURCE_DIR}/output/")