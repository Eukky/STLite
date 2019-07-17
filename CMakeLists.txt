cmake_minimum_required(VERSION 2.8)

project(STLite)

set(STLite_VERSION_MAJOR 1)
set(STLite_VERSION_MINOR 0)
set(STLite_VERSION_PATCH 0)
set(STLite_VERSION "${STLite_VERSION_MAJOR}.${STLite_VERSION_MINOR}.${STLite_VERSION_PATCH}")
message(STATUS "The version of this project is: ${STLite_VERSION}")

set(CMAKE_BUILD_TYPE release)

