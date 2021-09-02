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
    public:
        struct reg{
            int addr;
            int data;
            enum access{
                read,
                write,
            } permission;

            reg() : addr(0), data(0), permission(access::read){}
            reg(int reg_addr, int reg_data)
                    : addr(reg_addr), data(reg_data), permission(access::read){}
            reg(int reg_addr, int reg_data, access perms)
                    : addr(reg_addr), data(reg_data), permission(perms){}
        };

        memoryController();

        ~memoryController();

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

        std::byte* m_virtual_memory;

        void init_registers();

        void init_ram();

        void init_A_bus();

        void init_B_bus();

        void init_data_bus();

        void setup_virtual_memory();

        bool write_byte(std::byte *mem, int addr, std::byte data);

        bool write_bytes(std::byte *mem, int start_addr, std::byte *data, int len);

        bool write_nulls(std::byte *mem, int start_addr, int len);

        bool write_string(std::byte *mem, int start_addr, std::string data);

        std::byte *get_byte(int addr);

        std::byte *ABus() const;

        std::byte *BBus() const;

        std::byte *DataBus() const;

        std::byte *Ram() const;
    };
}
#endif //SNEKS_MEMORYCONTROLLER_H
