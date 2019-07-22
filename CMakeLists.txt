cmake_minimum_required(VERSION 2.8)

project(stlite)

set(STLite_VERSION_MAJOR 1)
set(STLite_VERSION_MINOR 0)
set(STLite_VERSION_PATCH 0)
set(STLite_VERSION "${STLite_VERSION_MAJOR}.${STLite_VERSION_MINOR}.${STLite_VERSION_PATCH}")
message(STATUS "The version of this project is: ${STLite_VERSION}")

set(CMAKE_BUILD_TYPE release)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(${PROJECT_SOURCE_DIR}/test)