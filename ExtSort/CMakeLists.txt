cmake_minimum_required(VERSION 2.8)
project(extsort)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(extsort main.cpp)

install(TARGETS extsort RUNTIME DESTINATION bin)
