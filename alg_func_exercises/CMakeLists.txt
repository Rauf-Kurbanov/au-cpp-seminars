cmake_minimum_required(VERSION 3.3)
project(alg_func_exercises)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(alg_func_exercises ${SOURCE_FILES})