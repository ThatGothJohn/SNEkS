#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include <stdexcept>

int main(int argc, char* argv[]) {
    std::string rom_name;
    if (argc < 2) {
        rom_name = "../roms/test.smc";
//        printf("\nExpected SNES rom filename as argument (eg .smc or .sfc)\n");
//        throw std::invalid_argument("Rom File not provided!");
    } else {
        rom_name = argv[1];
    }

    auto* main_cpu = new cpu::Cpu;
    if(!main_cpu->load_rom(rom_name.c_str())) {
        printf("\nCpu crashed while loading Rom!\n");
        return 1;
    }

    //main_cpu->log_stats();


    delete main_cpu;
    return 0;
}
