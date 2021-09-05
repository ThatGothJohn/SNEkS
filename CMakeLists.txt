cmake_minimum_required(VERSION 3.0)
project(sneks VERSION 0.1.0)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL2 -pthread")


add_executable(sneks main.cpp cpu.h ppu.h apu.h cpu.cpp memoryController.cpp memoryController.h ppu.cpp)
target_link_libraries(sneks ${SDL2_LIBRARIES})
