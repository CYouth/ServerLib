cmake_minimum_required(VERSION 3.1)
project(muduo)
set(CMAKE_CXX_STANDARD 11)	 # ����C++��׼Ϊ C++ 11
set(SRC_LIST Acceptor.cpp Channel.cpp Epoll.cpp EventLoop.cpp main.cpp TcpConnection.cpp TcpServer.cpp)
add_executable(muduo ${SRC_LIST})