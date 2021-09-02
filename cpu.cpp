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
        std::printf("Ram:\n");
        auto virtual_mem = this->m_memory_controller.VirtualMemory();

        for (int x = 0x0000; x < 0x10000; x++) {
            if (x % 4 == 0 && x != 0)
                std::printf(" ");
            if (x % 64*8 == 0 && x != 0)
                std::printf("\n");
            std::printf("%02X", virtual_mem[x]);
        }
        std::printf("\nRegisters:\n");
        for(auto const& [key, val] : this->m_memory_controller.Registers())
            printf("reg %s: 0x%02X, perms: %i, Virt addr: 0x%02X\n", key.c_str(), val.data, val.permission, val.addr);
    }
}
