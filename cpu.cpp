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

    void Cpu::log_stats() {
        std::printf("Virtual Memory:\n");
        auto virtual_mem = this->m_memory_controller.VirtualMemory();

        for (int x = 0x0000; x < 0x10000; x++) {
            if (x % 4 == 0 && x != 0)
                std::printf(" ");
            if (x % 64*8 == 0 && x != 0)
                std::printf("\n");
            std::printf("%02X", virtual_mem[x]);
        }
        std::printf("\nPPU Memory:\n");
        auto PPU_ram = this->m_memory_controller.PPU_Ram();

        for (int x = 0x0000; x < 64 * 1024; x++) {
            if (x % 4 == 0 && x != 0)
                std::printf(" ");
            if (x % 64*8 == 0 && x != 0)
                std::printf("\n");
            std::printf("%02X", PPU_ram[x]);
        }
        std::printf("\nRegisters:\n");
        for(auto const& [key, val] : this->m_memory_controller.Registers())
            printf("reg %s: 0x%02X, perms: %i, Virt addr: 0x%02X\n", key.c_str(), val.data, val.permission, val.addr);
    }

    bool Cpu::load_rom(const char* filename) {
        FILE* rom_file;
        rom_file = fopen(filename, "r");
        if (!rom_file) {
            printf("\nRom File not found or is busy!\n");
            return false;
        }
        fseek (rom_file, 0, SEEK_END);
        off_t sizeOfFile = ftell(rom_file);
        if (fseek(rom_file, 0, SEEK_SET) != 0 || sizeOfFile == 0) {
            printf("\nfseek failed!\n");
            return false;
        }
        unsigned char* rom_contents = (unsigned char*)malloc(sizeOfFile + 1);

        if (rom_contents == NULL){
            printf("\nFailed to allocate memory for the Rom file!\n");
            return false;
        }
        if (fread(rom_contents, 1, sizeOfFile, rom_file) == 0){
            printf("\nFailed to read the Rom file! file size: %li, filename : %s\n", sizeOfFile, filename);
            return false;
        }
        this->m_memory_controller.load_rom_into_virtual_memory(rom_contents, sizeOfFile);
        return true;
    }
}
