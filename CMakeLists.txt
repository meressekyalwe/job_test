cmake_minimum_required(VERSION 3.0.0)
project(job_test VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

if(POLICY CMP0079)
  cmake_policy(SET CMP0079 NEW)
endif()

add_subdirectory(src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(OpenCV REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})

