cmake_minimum_required(VERSION 3.6)
project(Durianver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(BOOST_ROOT "/usr/include/boost")
include_directories(/usr/include)
link_directories(/usr/lib)

set(SOURCE_FILES src/main.cpp src/duepoll.cpp src/duepoll.h src/threadpoll.h src/blockingqueue.h)
add_executable(Durianver ${SOURCE_FILES})

