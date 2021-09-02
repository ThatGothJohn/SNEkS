#include "cpu.h"
#include "ppu.h"
#include "apu.h"

int main() {
    cpu::Cpu* main_cpu = new cpu::Cpu;

    main_cpu->log_stats();
    return 0;
}
