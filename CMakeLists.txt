cmake_minimum_required(VERSION 3.5)
project(Durianver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pthread")

set(BOOST_ROOT "/usr/include/boost")
include_directories(/usr/include)
link_directories(/usr/lib)

set(SOURCE_FILES src/main.cpp src/logging.cpp src/logging.h src/loop.cpp src/loop.h src/tcpserver.cpp src/tcpserver.h src/epollevent.cpp src/epollevent.h src/socketwrap.cpp src/socketwrap.h src/acceptor.cpp src/acceptor.h src/tcpConnection.cpp src/tcpConnection.h src/socket.cpp src/socket.h)
add_executable(Durianver ${SOURCE_FILES})

