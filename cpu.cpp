//
// Created by john on 01/09/2021.
//
#include "cpu.h"

#include <cstdio>
#include <cstdlib>


namespace cpu {

    Cpu::Cpu(memory::memoryController MemoryController) {
        this->m_memory_controller = MemoryController;
        this->init_instructions();
    }
    Cpu::~Cpu() {
        this->m_memory_controller = {};
    }
    void Cpu::log_stats() {
        auto cart_info = this->m_memory_controller.Cart_info();
        printf("Rom name: %s\n", std::string(cart_info["TITLE"].data, cart_info["TITLE"].size).c_str());
//        printf("Version: %s\n", std::string(cart_info["VERSION"].data, cart_info["VERSION"].size).c_str());
//        printf("Rom size: %u\n", std::string(cart_info["ROMSIZE"].data, cart_info["ROMSIZE"].size).c_str());
//        printf("SRAM size: %u\n", cart_info["RAMSIZE"].data);

        std::printf("Virtual Memory:\n");
        auto virtual_mem = this->m_memory_controller.VirtualMemory();

        for (int x = 0x0000; x < 0x10000; x++) {
            if (x % 4 == 0 && x != 0)
                std::printf(" ");
            if (x % 64*8 == 0 && x != 0)
                std::printf("\n");
            std::printf("%02hhX", virtual_mem[x]);
        }
//        std::printf("\nPPU Memory:\n");
//        auto PPU_ram = this->m_memory_controller.PPU_Ram();
//
//        for (int x = 0x0000; x < 64 * 1024; x++) {
//            if (x % 4 == 0 && x != 0)
//                std::printf(" ");
//            if (x % 64*8 == 0 && x != 0)
//                std::printf("\n");
//            std::printf("%02hhX", PPU_ram[x]);
//        }
        std::printf("\nRegisters:\n");
        auto regis = this->m_memory_controller.Registers();
        for(auto const& [key, val] : regis)
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
        auto* rom_contents = (char*)malloc(sizeOfFile + 1);

        if (rom_contents == nullptr){
            printf("\nFailed to allocate memory for the Rom file!\n");
            return false;
        }
        if (fread(rom_contents, 1, sizeOfFile, rom_file) == 0){
            printf("\nFailed to read the Rom file! file size: %li, filename : %s\n", sizeOfFile, filename);
            return false;
        }
        if (!this->m_memory_controller.load_rom_into_virtual_memory(rom_contents, sizeOfFile)){
            printf("Rom name: %s\t Rom size: %li", filename, sizeOfFile);
            return false;
        }

        return true;
    }

    void Cpu::init_instructions() {
        this->instructions[0x61].callback = [this]{ printf("test: %s\n", this->instructions[0x61].friendly_name.c_str()); return true;}; //todo: replace with actual implementations of instructions
    }

    error_t run(){

        return 0;
    }
}
