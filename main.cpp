#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include <stdexcept>

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("\nExpected SNES rom filename as argument\n");
        throw std::invalid_argument("Rom File not provided!");
    }
    FILE* rom_file;
    rom_file = fopen(argv[1], "r");
    if (!rom_file) {
        printf("\nRom File not found or is busy!\n");
        throw std::invalid_argument("Rom File not found!");
    }
    fseek (rom_file, 0, SEEK_END);
    off_t sizeOfFile = ftell(rom_file);
    if (fseek(rom_file, 0, SEEK_SET) != 0 || sizeOfFile == 0) {
        printf("\nfseek failed!\n");
        throw std::invalid_argument("Fseek!");
    }
    char* result = (char*)malloc(sizeOfFile/8 + 1);

    if (result == NULL){
        printf("\nFailed to allocate memory for the Rom file!\n");
        throw std::invalid_argument("No memory!");
    }
    if (fread(result, 1, sizeOfFile/8, rom_file) == 0){
        printf("\nFailed to read the Rom file! file size: %li, filename : %s\n", sizeOfFile, argv[1]);
        throw std::invalid_argument("Oh Noes!");
    }
    cpu::Cpu* main_cpu = new cpu::Cpu;
    main_cpu->load_rom(result, sizeOfFile);

    main_cpu->log_stats();

    delete main_cpu;
    return 0;
}
