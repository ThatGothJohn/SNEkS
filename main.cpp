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

    cpu::Cpu* main_cpu = new cpu::Cpu;
    main_cpu->load_rom(argv[1]);

    main_cpu->log_stats();


    delete main_cpu;
    return 0;
}
