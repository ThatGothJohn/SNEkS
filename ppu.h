//
// Created by john on 01/09/2021.
//
#ifndef SNEKS_PPU_H
#define SNEKS_PPU_H

#include "memoryController.h"

#include <SDL2/SDL.h>
#include <thread>


namespace ppu {

    class PPU {
    private:

        memory::memoryController* m_memory_controller;

    public:
        PPU();
        PPU(memory::memoryController**);
        ~PPU();

        std::thread run();
    };
}
#endif //SNEKS_PPU_H
