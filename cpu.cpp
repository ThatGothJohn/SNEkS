//
// Created by john on 01/09/2021.
//
#include "cpu.h"

#include <iostream>
#include "cstring"


namespace cpu {

    Cpu::Cpu() {
        this->m_memory_controller = memory::memoryController();

        //memory::write_string(this->m_ram, 0, "This is a Test!!!!!");
    }
    Cpu::~Cpu() {
        this->m_memory_controller = {};
    }
    void Cpu::init() {

    }
    void Cpu::log_stats() {
        std::printf("1st kB of Ram:\n");
        auto ram_cached = this->m_memory_controller.Ram();
        for (int x = 0x0000; x < 1024; x++) {
            if (x % 4 == 0 && x != 0)
                std::printf(" ");
            if (x % 64*8 == 0 && x != 0)
                std::printf("\n");
            std::printf("%02X", ram_cached[x]);
        }
        std::printf("\nRegisters:\n");
        for(auto const& [key, val] : this->m_memory_controller.Registers())
            printf("reg %s: %02X\n", key.c_str(), val.data);
    }
}
