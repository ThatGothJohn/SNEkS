//
// Created by john on 01/09/2021.
//
#ifndef SNEKS_CPU_H
#define SNEKS_CPU_H

#include <cstddef>
#include <string>
#include <map>
#include <functional>
#include <cstdio>
#include <cstdlib>
#include <thread>


#include "memoryController.h"

namespace cpu {

//TODO: instruction handling,
//      boot loader
    class CPU {
    private:

        struct instruction {
            std::string assembly; //the assembly for the instruction, much nicer way of displaying it
            std::string friendly_name; //helpful for later/debugging :)
            int bytes;  //how many bytes in memory this instruction is, including operands
            int base_cycles; //may take more with going over page boundaries etc
            std::function<bool(void)> callback; //where the actual functionality of this instruction will be in the emulator code :)
        };

        memory::memoryController* m_memory_controller;

        struct internal_register {
            std::string mnemonic;   //what the reg is usually referred to
            std::string friendly_name;  //a nicer name
            char16_t data;  //the actual registers storage
        };

        std::map<std::string, internal_register> cpu_registers = {  //https://en.wikibooks.org/wiki/Super_NES_Programming/65c816_reference
                {"A", {"A", "Accumulator", (char16_t) 0x0000}},
                {"X", {"X", "Index X", (char16_t) 0x0000}},
                {"Y", {"Y", "Index Y", (char16_t) 0x0000}},
                {"S", {"S", "Stack Pointer", (char16_t) 0x0000}},
                {"DBR", {"DBR", "Data Bank", (char16_t) 0x0000}},
                {"D", {"D", "Direct page", (char16_t) 0x0000}},
                {"PBR", {"PBR", "program Bank", (char16_t) 0x0000}},
                {"PC", {"PC", "Program Counter", (char16_t) 0x0000}},
                //Processor status holds a lot, see comment below, after the cpu_registers definition
                {"P", {"P", "Processor Status", (char16_t) 0x0000}},
        };
        //Processor status:
        //

        //instructions and friendly names from https://wiki.superfamicom.org/65816-reference
        std::map<char, instruction> instructions = {
                //can be referenced by this->instructions[0xXX] and ran with this->instructions[0xXX].callback(); Hopefully making this easy when we start running code from vmem
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
                {0x77, {"ADC", "Add with carry (DP Indirect Long Indexed,Y)", 2, 6, []{ printf("0x77 not implemented;\n"); return false; }}},
                {0x79, {"ADC", "Add with carry (Absolute Indexed,Y)", 3, 4, []{ printf("0x79 not implemented;\n"); return false; }}},
                {0x7D, {"ADC", "Add with carry (Absolute Indexed,X)", 3, 4, []{ printf("0x7D not implemented;\n"); return false; }}},
                {0x7F, {"ADC", "Add with carry (Absolute Long Indexed,X)", 4, 5, []{ printf("0x7F not implemented;\n"); return false; }}},

                {0x21, {"AND", "AND Accumulator with Memory (DP Indexed Indirect,X)", 2, 6, []{ printf("0x21 not implemented;\n"); return false; }}},
                {0x23, {"AND", "AND Accumulator with Memory (Stack Relative)", 2, 4, []{ printf("0x23 not implemented;\n"); return false; }}},
                {0x25, {"AND", "AND Accumulator with Memory (Direct Page)", 2, 3, []{ printf("0x25 not implemented;\n"); return false; }}},
                {0x27, {"AND", "AND Accumulator with Memory (DP Indirect Long)", 2, 6, []{ printf("0x27 not implemented;\n"); return false; }}},
                {0x29, {"AND", "AND Accumulator with Memory (Immediate)", 2, 2, []{ printf("0x29 not implemented;\n"); return false; }}},
                {0x2D, {"AND", "AND Accumulator with Memory (Absolute)", 3, 4, []{ printf("0x2D not implemented;\n"); return false; }}},

                {0x50, {"BVC", "Branch if Overflow Clear", 2, 2, []{ printf("0x50 not implemented;\n"); return false; }}},

                {0x7B, {"TDC", "Transfer Direct Page REgister to 16-bit accumulator", 1, 2, []{ printf("0x7B not implemented;\n"); return false; }}},

        };

        long m_clock_rate = 21477270; //Hertz

        void init_instructions();

        void init();

    public:
        CPU() = default;
        CPU(memory::memoryController**);
        ~CPU();
        bool load_rom(const char*);
        std::thread run();
        void log_stats();
    };


}

#endif //SNEKS_CPU_H
