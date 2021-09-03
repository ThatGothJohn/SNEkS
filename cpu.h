//
// Created by john on 01/09/2021.
//
#include <cstddef>
#include <string>
#include <map>
#include <functional>
#include "memoryController.h"

#ifndef SNEKS_CPU_H
#define SNEKS_CPU_H

namespace cpu {

//TODO: instruction handling,
//      boot loader
    class Cpu {
    private:

        struct instruction {
            std::string assembly; //the assembly for the instruction, much nicer way of displaying it
            std::string friendly_name; //helpful for later/debugging :)
            int bytes;  //how many bytes in memory this instruction is, including operands
            int base_cycles; //may take more with going over page boundaries etc
            std::function<bool(void)> callback; //where the actual functionality of this instruction will be in the emulator code :)
        };

        memory::memoryController m_memory_controller;

        struct internal_register {
            std::string mnemonic;   //what the reg is usually referred to
            std::string friendly_name;  //a nicer name
            char data;  //the actual registers storage
        };

        std::map<std::string, internal_register> cpu_registers = {
                {"A", {"A", "Accumulator", (char) 0x00}},
                {"X", {"X", "Index X", (char) 0x00}},
                {"Y", {"Y", "Index Y", (char) 0x00}},
                {"SP", {"SP", "Stack Pointer", (char) 0x00}},
                {"DB", {"DB", "Data Bank", (char) 0x00}},
                {"P", {"P", "Processor Status", (char) 0x00}},
                {"PC", {"PC", "Program Counter", (char) 0x00}},
        };

        //instructions and friendly names from https://wiki.superfamicom.org/65816-reference
        std::map<char, instruction> instructions = { //can be referenced by this->instructions[0xXX] and ran with this->instructions[0xXX].callback();
                {0x61, {"ADC", "Add with carry (DP indexed indirect, X)", 2, 6, []{ printf("0x61 not implemented;\n"); return false; }}},
                {0x63, {"ADC", "Add with carry (Stack relative)", 2, 4, []{ printf("0x63 not implemented;\n"); return false; }}},
                {0x65, {"ADC", "Add with carry (Direct Page)", 2, 3, []{ printf("0x65 not implemented;\n"); return false; }}},
                {0x67, {"ADC", "Add with carry (DP Indirect Long)", 2, 6, []{ printf("0x67 not implemented;\n"); return false; }}},
                {0x69, {"ADC", "Add with carry (Immediate)", 2, 2, []{ printf("0x69 not implemented;\n"); return false; }}},
                {0x6D, {"ADC", "Add with carry (Absolute)", 3, 4, []{ printf("0x6D not implemented;\n"); return false; }}},
                {0x6F, {"ADC", "Add with carry (Absolute Long)", 4, 5, []{ printf("0x6F not implemented;\n"); return false; }}},
                {0x71, {"ADC", "Add with carry (DP Indirect Indexed, Y)", 2, 5, []{ printf("0x71 not implemented;\n"); return false; }}},
                {0x72, {"ADC", "Add with carry (DP Indirect)", 2, 5, []{ printf("0x72 not implemented;\n"); return false; }}},
                {0x73, {"ADC", "Add with carry (SR Indirect Indexed,Y)", 2, 7, []{ printf("0x73 not implemented;\n"); return false; }}},
                {0x75, {"ADC", "Add with carry (DP Indexed,X)", 2, 4, []{ printf("0x75 not implemented;\n"); return false; }}},
        };

        long m_clock_rate = 21477270; //Hertz
        enum m_clock_divider_for_bus {
            a=6,
            b=8,
            c=12,
        };

        void init_instructions();

    public:
        Cpu();
        ~Cpu();
        bool load_rom(const char*);
        void log_stats();
    };


}

#endif //SNEKS_CPU_H
