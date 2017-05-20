cmake_minimum_required(VERSION 3.6)
project(logtest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

aux_source_directory(../../src SOURCE_FILES)

set(SOURCE_FILES main.cpp ../../src/logging.cpp)
add_executable(logtest ${SOURCE_FILES})