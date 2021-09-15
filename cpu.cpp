//
// Created by john on 01/09/2021.
//
#include "cpu.h"



namespace cpu {

    CPU::CPU(memory::memoryController** MemoryController) : m_memory_controller(*MemoryController){
        this->init_instructions();
    }
    CPU::~CPU() = default;
    void CPU::log_stats() {
        auto cart_info = this->m_memory_controller->Cart_info();
        auto cart = this->m_memory_controller->game_cart;
        printf("Rom name: %s\n", std::string(cart_info["TITLE"].data, cart_info["TITLE"].size).c_str());
//        printf("Version: %s\n", std::string(cart_info["VERSION"].data, cart_info["VERSION"].size).c_str());
        printf("Rom size: %uBytes\n", 0x400<<cart[cart_info["ROMSIZE"].addr]);
        printf("SRAM size: %uBytes\n", 0x400<<cart[cart_info["RAMSIZE"].addr]);

        printf("first 10 bytes of cart:\n");
        for(int x = 0; x< 10; x++)
            printf("$%02X\t", cart[x]);
        printf("\n");

//        std::printf("Virtual Memory 0x800000-0x900000:\n");
//        auto virtual_mem = this->m_memory_controller.VirtualMemory();
//
//        for (int x = 0x000000; x < 0x2000; x++) {
//            if (x % 4 == 0 && x != 0)
//                std::printf(" ");
//            if (x % 64*8 == 0 && x != 0)
//                std::printf("\n");
//            std::printf("%02hhX", virtual_mem[0xF00000+x]);
//        }
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
//        std::printf("\nRegisters:\n");
//        auto regis = this->m_memory_controller.Registers();
//        for(auto const& [key, val] : regis)
//            printf("reg %s: 0x%02X, perms: %i, Virt addr: 0x%02X\n", key.c_str(), val.data, val.permission, val.addr);

    }

    bool CPU::load_rom(const char* filename) {
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
        if (!this->m_memory_controller->load_rom_into_virtual_memory(rom_contents, sizeOfFile)){
            printf("Rom name: %s\t Rom size: %li", filename, sizeOfFile);
            return false;
        }

        this->init();

        return true;
    }

    void CPU::init_instructions() { //important instruction resource: https://emudev.de/q00-snes/65816-the-cpu/
        //todo: implementations of instructions
        //      generalize certain instructions AKA ADC and avoid repeated implementation code
        this->instructions[0x9c].callback = [this]{
            auto vmem = this->m_memory_controller->VirtualMemory();
            auto addr = vmem[this->cpu_registers["PC"].data+2]<<8 | vmem[this->cpu_registers["PC"].data+1];
            printf("Set $%04X to Zero\n", addr); //todo: bus stuff
            this->m_memory_controller->VirtualMemory()[addr] = 0x00;
            this->clocks = this->instructions[0x9c].base_cycles + ((this->cpu_registers["P"].data & 0x20) == 0x20);
            this->cpu_registers["PC"].data += this->instructions[0x9c].bytes;
            return true;
        };
        this->instructions[0x78].callback = [this]{
            this->cpu_registers["P"].data | 0x04; //set Interrupt disable Flag
            this->clocks = this->instructions[0x78].base_cycles;
            this->cpu_registers["PC"].data += this->instructions[0x78].bytes;
            printf("Set Interrupt disable flag\n");
            return true;
        };
        this->instructions[0x80].callback = [this]{
            auto vmem = this->m_memory_controller->VirtualMemory();
            auto jump_size = vmem[this->cpu_registers["PC"].data + 1];
            printf("Jump by $%02X from $%06X\t", jump_size, this->cpu_registers["PC"].data);
            this->cpu_registers["PC"].data += this->instructions[0x80].bytes + jump_size;
            printf("Landed at $%06X\n",this->cpu_registers["PC"].data);
            return true;
        };
    }

    void CPU::init() {
        auto reset = this->m_memory_controller->Cart_info()["RESET"].data;
        this->cpu_registers["PC"].data = reset[1]<<8 | reset[0];
        this->cpu_registers["SP"].data = 0x000001FC;
    }

    std::thread CPU::run(){
        std::thread runtime([this](){
            while (true){
                auto pc = this->cpu_registers["PC"];
                printf("PC: %04X\n", pc.data);
                auto current_instruction = this->m_memory_controller->get_byte(pc.data);
                if (!this->instructions.contains(current_instruction)){
                    printf("Instruction %02X not implemented!!!\tPC: $%06X\n", current_instruction, pc.data);
                    return 1;
                }
                auto instruction = this->instructions[current_instruction];
                auto operandbytes = instruction.bytes;
                auto* operands = new char8_t[operandbytes-1];
                for (int x = 1; x<operandbytes;x++){
                    operands[x-1] = this->m_memory_controller->VirtualMemory()[pc.data+x];
                }
                bool success = instruction.callback();  //todo: pass in operands here instead of getting them in the instructions callback

                delete[] operands;
                if (!success)
                    return 1;
                //todo: handle clock cycles
            }
        });
        return runtime;
    }
}
