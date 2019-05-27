cmake_minimum_required(VERSION 3.9)
project(part1)

set(CMAKE_CXX_STANDARD 11)

add_executable(part1 part1_main.cpp Headers.hpp Semaphore.hpp Semaphore.cpp PCQueue.cpp PCQueue.hpp)