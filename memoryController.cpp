//
// Created by john on 02/09/2021.
//

#include "memoryController.h"
#include "cpu.h"

namespace memory {
    memoryController::memoryController(){
        this->init_ram();
        this->init_data_bus();
        this->init_A_bus();
        this->init_B_bus();
        this->init_registers();
        this->setup_virtual_memory();
    }

    memoryController::~memoryController() {}

    std::byte *memoryController::ABus() const {
        return m_A_bus;
    }

    std::byte *memoryController::BBus() const {
        return m_B_bus;
    }

    std::byte *memoryController::DataBus() const {
        return m_data_bus;
    }

    std::byte *memoryController::Ram() const {
        return m_ram;
    }

    void memoryController::init_registers() {
        //names from https://wiki.superfamicom.org/registers
        this->m_registers = {
                {"NMITIMEN",reg(0x4200, 0x00, reg::access::write)},      //interrupt enable
                {"WRIO",reg(0x4201, 0x00, reg::access::write)},          //io port write
                {"WRMPYA",reg(0x4202, 0x00, reg::access::write)},        //multiplicand A
                {"WRMPYB", reg(0x4203, 0x00, reg::access::write)},       //multiplicand B
                {"WRDIVL", reg(0x4204, 0x00, reg::access::write)},       //divisor & dividend low
                {"WRDIVH", reg(0x4205, 0x00, reg::access::write)},       //divisor & dividend high
                {"WRDIVB", reg(0x4206, 0x00, reg::access::write)},       //divisor & dividend B
                {"HTIMEL", reg(0x4207, 0x00, reg::access::write)},       //IRQ timer horizontal low
                {"HTIMEH", reg(0x4208, 0x00, reg::access::write)},       //IRQ timer horizontal high
                {"VTIMEL", reg(0x4209, 0x00, reg::access::write)},       //IRQ timer vertical low
                {"VTIMEH", reg(0x420A, 0x00, reg::access::write)},       //IRQ timer vertical high
                {"MDMAEN", reg(0x420B, 0x00, reg::access::write)},       //DMA enable
                {"HDMAEN", reg(0x420C, 0x00, reg::access::write)},       //HDMA enable
                {"MEMSEL", reg(0x420D, 0x00, reg::access::write)},       //ROM Speed
                {"RDNMI", reg(0x4210, 0x00)},                                   //interrupt flag A
                {"TIMEUP", reg(0x4211, 0x00)},                                  //interrupt flag B
                {"HVBJOY", reg(0x4212, 0x00)},                                  //PPU Status
                {"RDIO", reg(0x4213, 0x00)},                                    //IO Port Read
                {"RDDIVL", reg(0x4214, 0x00)},                                  //divide result low
                {"RDDIVH", reg(0x4215, 0x00)},                                  //divide result high
        };
    }

    void memoryController::init_ram() {
        this->m_ram = new std::byte[128 * 1024];
    }

    void memoryController::init_A_bus() {
        this->m_A_bus = new std::byte[3]; //24-bit
    }

    void memoryController::init_B_bus() {
        this->m_B_bus = new std::byte[1]; //8-bit
    }

    void memoryController::init_data_bus() {
        this->m_data_bus = new std::byte[1]; //8-bit
    }

    void memoryController::setup_virtual_memory() { //http://graphics.stanford.edu/~ianbuck/proj/Nintendo/node6.html
        this->m_virtual_memory = new std::byte[0x10000];
        for (int x = 0x0000; x < 0x0800; x++)
            this->m_virtual_memory[x] = m_ram[x];
        for (int x = 0x0000; x < 0x0800; x++)
            this->m_virtual_memory[x+0x0800] = m_ram[x];
        for (int x = 0x0000; x < 0x0800; x++)
            this->m_virtual_memory[x+0x1000] = m_ram[x];
        for (int x = 0x0000; x < 0x0800; x++)
            this->m_virtual_memory[x+0x1800] = m_ram[x];
        for (auto const& [key, val] : this->m_registers)
            this->m_virtual_memory[val.addr] = (std::byte)val.data;
        for (int x = 0x0000; x < 0x1000; x++) //placeholder for expansion modules
            this->m_virtual_memory[0x5000+x] = (std::byte)0x00;
        for (int x = 0x0000; x < 0x2000; x++) //placeholder for battery backed ram
            this->m_virtual_memory[0x6000 + x] = (std::byte)0x00;
        for (int x = 0x0000; x < 0x8000; x++) //placeholder for rom
            this->m_virtual_memory[0x8000+x] = (std::byte)0x00;
    }

    bool memoryController::write_byte(std::byte *mem, int addr, std::byte data) {
        mem[addr] = data;
        return true;
    }

    bool memoryController::write_bytes(std::byte *mem, int start_addr, std::byte *data, int len) {
        for (int x = 0; x < len; x++)
            mem[start_addr + x] = data[x];
        return true;
    }

    bool memoryController::write_nulls(std::byte *mem, int start_addr, int len){
        for (int x = 0; x < len; x++)
            mem[start_addr + x] = (std::byte)0x00;
        return true;
    };

    bool memoryController::write_string(std::byte *mem, int start_addr, std::string data) {
        int len = data.length();
        std::byte *str_data = (std::byte *) data.c_str();
        return write_bytes(mem, start_addr, str_data, len);
    }

    std::byte *memoryController::get_byte(int addr) {
        //memory architecture from http://graphics.stanford.edu/~ianbuck/proj/Nintendo/node6.html

        if (addr < 0x0800) //internal Ram
            return 0;

    };
}