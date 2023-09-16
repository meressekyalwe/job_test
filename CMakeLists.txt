cmake_minimum_required(VERSION 3.0.0)
project(job_test VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(OpenCV REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV})
target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})

