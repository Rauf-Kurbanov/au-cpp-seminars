cmake_minimum_required(VERSION 3.5)
project(sfinae_serialize)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/test.cpp src/serialization.h)
add_executable(sfinae_serialize ${SOURCE_FILES})