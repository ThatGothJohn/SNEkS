//
// Created by john on 01/09/2021.
//
#include <cstddef>
#include <string>
#include <map>
#include "memoryController.h"

#ifndef SNEKS_CPU_H
#define SNEKS_CPU_H

namespace cpu {

//TODO instruction handling
//rom loading
//boot loader
    class Cpu {
    private:


        memory::memoryController m_memory_controller;


        long m_clock_rate = 21477270; //Hertz
        enum m_clock_divider_for_bus {
            a=6,
            b=8,
            c=12,
        };

    public:
        Cpu();
        ~Cpu();
        bool load_rom(char*, long int);
        void log_stats();
    };


}

#endif //SNEKS_CPU_H
