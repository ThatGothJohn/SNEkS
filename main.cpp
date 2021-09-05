#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include <stdexcept>


int main(int argc, char* argv[]) {
    std::string rom_name;
    if (argc < 2) {
        rom_name = "../roms/test.smc";
//        printf("\nExpected SNES rom filename as argument (eg .smc or .sfc)\n"); fixme:
//        throw std::invalid_argument("Rom File not provided!");
    } else {
        rom_name = argv[1];
    }
    auto memCtl = new memory::memoryController();

    auto* main_cpu = new cpu::CPU(&memCtl);
    auto* main_ppu = new ppu::PPU(&memCtl);


    if(!main_cpu->load_rom(rom_name.c_str())) {
        printf("\nCpu crashed while loading Rom!\n");
        return 1;
    }

    main_cpu->log_stats();

    auto cpu_runtime = main_cpu->run();
    auto ppu_runtime = main_ppu->run();

    ppu_runtime.join();
    cpu_runtime.join();

    delete main_cpu;
    delete main_ppu;
    delete memCtl;
    return 0;
}
