//
// Created by john on 02/09/2021.
//

#ifndef SNEKS_MEMORYCONTROLLER_H
#define SNEKS_MEMORYCONTROLLER_H

#include <cstddef>
#include <string>
#include <map>

namespace memory {

    class memoryController {
    private:

        struct snes_header {
            char     name[21];
            uint8_t  map_mode;   // xxAAxxxB
            uint8_t  rom_type;
            uint8_t  rom_size;   // 09,0a,0b,0c,0d [2048^val]
            uint8_t  sram_size;
            uint8_t  dest_code;
            uint8_t  fixed;
            uint8_t  version;
            uint16_t cmc_check;
            uint16_t cksum;
        };

        struct reg{
            int addr;
            int data;
            enum access{
                read=1,
                write=2,
                double_byte_write=4,
                word=8,
            } permission;

            reg() : addr(0), data(0), permission(access::read){}
            reg(int reg_addr, int reg_data)
                    : addr(reg_addr), data(reg_data), permission(access::read){}
            reg(int reg_addr, int reg_data, access perms)
                    : addr(reg_addr), data(reg_data), permission(perms){}
        };

        //         24-bit
        int m_A_bus_len = 3;
        std::byte *m_A_bus;
        //         8-bit
        int m_B_bus_len = 1;

        std::byte *m_B_bus;
        //         8-bit
        int m_data_bus_len = 1;
        std::byte *m_data_bus;
        //         128kB
        std::byte *m_ram;

        std::map<std::string, reg> m_registers;

        std::byte *m_PPU_ram;

        std::byte* m_virtual_memory;
    public:
        memoryController();

        ~memoryController();

        void init_registers();

        void init_ram();

        void init_A_bus();

        void init_B_bus();

        void init_data_bus();

        void init_PPU_ram();

        void setup_virtual_memory();

        bool write_byte(std::byte *mem, int addr, std::byte data);

        bool write_bytes(std::byte *mem, int start_addr, std::byte *data, int len);

        bool write_nulls(std::byte *mem, int start_addr, int len);

        bool write_string(std::byte *mem, int start_addr, std::string data);

        std::byte *ABus() const;

        std::byte *BBus() const;

        std::byte *DataBus() const;

        const std::map<std::string, reg> Registers() const;

        std::byte* PPU_Ram() const;

        std::byte *VirtualMemory() const;

        std::pair<std::string, reg> Register_from_address (int addr);

        bool load_rom_into_virtual_memory(unsigned char*, long int);

    };
}
#endif //SNEKS_MEMORYCONTROLLER_H
