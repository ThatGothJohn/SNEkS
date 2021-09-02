cmake_minimum_required(VERSION 3.0)
project(sneks)

set(CMAKE_CXX_STANDARD 20)

add_executable(sneks main.cpp cpu.h ppu.h apu.h cpu.cpp memoryController.cpp memoryController.h)
