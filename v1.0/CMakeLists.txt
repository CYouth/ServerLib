cmake_minimum_required(VERSION 3.1)
project(muduo)
set(CMAKE_CXX_STANDARD 11)	 # 设置C++标准为 C++ 11
set(SRC_LIST Acceptor.cpp Channel.cpp Epoll.cpp EventLoop.cpp main.cpp TcpConnection.cpp TcpServer.cpp)
add_executable(muduo ${SRC_LIST})