cmake_minimum_required(VERSION 3.9)
project(HW3)

set(CMAKE_CXX_STANDARD 11)


add_compile_options(-g -O3 -Wall -static-libstdc++)

add_executable(HW3 main.cpp
        Headers.hpp
        utils.hpp utils.cpp
        Thread.hpp
        PCQueue.hpp
        Semaphore.cpp Semaphore.hpp
        Board.cpp Board.hpp
        Game.hpp Game.cpp
        )