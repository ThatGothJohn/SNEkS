//
// Created by john on 02/09/2021.
//

#include "memoryController.h"
#include "cpu.h"

namespace memory {
    memoryController::memoryController() {
        this->init_ram();
        this->init_PPU_ram();
        this->init_data_bus();
        this->init_A_bus();
        this->init_B_bus();
        this->init_registers();
        this->setup_virtual_memory();
    }

    memoryController::~memoryController() = default;


    void memoryController::init_registers() {
        //names from https://wiki.superfamicom.org/registers & http://baltimorebarcams.com/eb/snes/docs/65816/SNES%20Registers.html
        this->m_registers = {
                {"INIDISP",     reg(0x2100, 0x00, reg::access::write)},      //screen display
                {"OBSEL",       reg(0x2101, 0x00, reg::access::write)},        //screen display
                {"OAMADDL",     reg(0x2102, 0x00, reg::access::write)},      //OAM address low
                {"OAMADDH",     reg(0x2103, 0x00, reg::access::write)},      //OAM address high
                {"OAMDATA",     reg(0x2104, 0x00, reg::access(reg::access::write | reg::access::word))},      //OAM data
                {"BGMODE",      reg(0x2105, 0x00, reg::access::write)},      //screen mode
                {"MOSAIC",      reg(0x2106, 0x00, reg::access::write)},      //screen pixelation
                {"BG1SC",       reg(0x2107, 0x00, reg::access::write)},      //BG1 VRAM location
                {"BG2SC",       reg(0x2108, 0x00, reg::access::write)},      //BG2 VRAM location
                {"BG3SC",       reg(0x2109, 0x00, reg::access::write)},      //BG3 VRAM location
                {"BG4SC",       reg(0x210A, 0x00, reg::access::write)},      //BG4 VRAM location
                {"BG12NBA",     reg(0x210B, 0x00, reg::access::write)},      //BG1 & BG2 VRAM location
                {"BG34NBA",     reg(0x210C, 0x00, reg::access::write)},      //BG3 & BG4 VRAM location
                {"BG1HOFS",     reg(0x210D, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},      //BG1 horizontal scroll
                {"BG1VOFS",     reg(0x210E, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},      //BG1 vertical scroll
                {"BG2HOFS",     reg(0x210F, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},      //BG2 horizontal scroll
                {"BG2VOFS",     reg(0x2110, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},      //BG2 vertical scroll
                {"BG3HOFS",     reg(0x2111, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},      //BG3 horizontal scroll
                {"BG3VOFS",     reg(0x2112, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},      //BG3 vertical scroll
                {"BG4HOFS",     reg(0x2113, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},      //BG4 horizontal scroll
                {"BG4VOFS",     reg(0x2114, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},      //BG4 vertical scroll
                {"VMAIN",       reg(0x2115, 0x00, reg::access::write)},      //video port control
                {"VMADDL",      reg(0x2116, 0x00, reg::access(
                        reg::access::write | reg::access::double_byte_write))},      //video port address low
                {"VMADDH",      reg(0x2117, 0x00, reg::access(
                        reg::access::write | reg::access::double_byte_write))},      //video port address high
                {"VMDATAL",     reg(0x2118, 0x00, reg::access(
                        reg::access::write | reg::access::double_byte_write))},      //video port data low
                {"VMDATAH",     reg(0x2119, 0x00, reg::access(
                        reg::access::write | reg::access::double_byte_write))},      //video port data high
                {"M7SEL",       reg(0x211A, 0x00, reg::access::write)},      //MODE7 settings
                {"M7A",         reg(0x211B, 0x00, reg::access::write)},      //COS rotate angle / X expansion
                {"M7B",         reg(0x211C, 0x00, reg::access::write)},      //SIN rotate angle / X expansion
                {"M7C",         reg(0x211D, 0x00, reg::access::write)},      //SIN rotate angle / Y expansion
                {"M7D",         reg(0x211E, 0x00, reg::access::write)},      //COS rotate angle / Y expansion
                {"M7X",         reg(0x211F, 0x00, reg::access(
                        reg::access::write | reg::access::double_byte_write))},      //Center position X
                {"M7Y",         reg(0x2120, 0x00, reg::access(
                        reg::access::write | reg::access::double_byte_write))},      //Center position Y
                {"CGADD",       reg(0x2121, 0x00, reg::access::write)},      //color # / palette selection
                {"CGDATA",      reg(0x2122, 0x00, reg::access(
                        reg::access::write | reg::access::double_byte_write))},      //color data
                {"W12SEL",      reg(0x2123, 0x00, reg::access::write)},      //Window mask settings
                {"W34SEL",      reg(0x2124, 0x00, reg::access::write)},      //window mask settings
                {"WOBJSEL",     reg(0x2125, 0x00, reg::access::write)},      //window mask settings
                {"WH0",         reg(0x2126, 0x00, reg::access::write)},      //window 1 left position
                {"WH1",         reg(0x2127, 0x00, reg::access::write)},      //window 1 right position
                {"WH2",         reg(0x2128, 0x00, reg::access::write)},      //window 2 left position
                {"WH3",         reg(0x2129, 0x00, reg::access::write)},      //window 2 right position
                {"WBGLOG",      reg(0x212A, 0x00,
                                    reg::access::write)},      //Mask logic settings for window 1 & 2 per screen
                {"WOBJLOG",     reg(0x212B, 0x00,
                                    reg::access::write)},      //Mask logic settings for color windows and obj windows
                {"TM",          reg(0x212C, 0x00, reg::access::write)},      //Main screen designation
                {"TD",          reg(0x212D, 0x00, reg::access::write)},      //Sub-screen designation
                {"TMW",         reg(0x212E, 0x00, reg::access::write)},      //window mask main screen designation
                {"TSW",         reg(0x212F, 0x00, reg::access::write)},      //window mask sub-screen designation
                {"CGWSEL",      reg(0x2130, 0x00,
                                    reg::access::write)},      //Fixed color addition or screen addition register
                {"CGADSUB",     reg(0x2131, 0x00,
                                    reg::access::write)},      //addition/subtraction for screens, BGs & OBJs
                {"COLDATA",     reg(0x2132, 0x00, reg::access::write)},      //fixed color data for fixed color +/-
                {"SETINI",      reg(0x2133, 0x00, reg::access::write)},      //screen mode/video select
                {"MPYL",        reg(0x2134, 0x00, reg::access::read)},      //Multiplication result low
                {"MPYM",        reg(0x2135, 0x00, reg::access::read)},      //Multiplication result mid
                {"MPYH",        reg(0x2136, 0x00, reg::access::read)},      //Multiplication result high
                {"SLHV",        reg(0x2137, 0x00,
                                    reg::access::read)},      //software latch for horizontal/vertical counter
                {"OAMDATAREAD", reg(0x2138, 0x00, reg::access::read)},      //read data from OAM
                {"VMDATALREAD", reg(0x2139, 0x00,
                                    reg::access(reg::access::read | reg::access::word))},      //read data from VRAM low
                {"VMDATAHREAD", reg(0x213A, 0x00, reg::access(
                        reg::access::read | reg::access::word))},      //read data from VRAM high
                {"CGDATAREAD",  reg(0x213B, 0x00, reg::access::read)},      //read data from CG-RAM (color)
                {"OPHCT",       reg(0x213C, 0x00, reg::access(reg::access::read |
                                                              reg::access::double_byte_write))},      //horizontal scanline location //maybe fixme 9 bit regs?
                {"OPVCT",       reg(0x213D, 0x00, reg::access(reg::access::read |
                                                              reg::access::double_byte_write))},      //horizontal scanline location //maybe fixme 9 bit regs?
                {"STAT77",      reg(0x213E, 0x00, reg::access::read)},      //PPU status flag and version number
                {"STAT78",      reg(0x213F, 0x00, reg::access::read)},      //PPU status flag and version number

                {"APUI00",      reg(0x2140, 0x00, reg::access(reg::access::read | reg::access::write))},      //Audio
                {"APUI01",      reg(0x2141, 0x00, reg::access(reg::access::read | reg::access::write))},
                {"APUI02",      reg(0x2142, 0x00, reg::access(reg::access::read | reg::access::write))},
                {"APUI03",      reg(0x2143, 0x00, reg::access(reg::access::read | reg::access::write))},

                {"WMDATA",      reg(0x2180, 0x00,
                                    reg::access(reg::access::read | reg::access::write))},      //read/write WRAM
                {"WMADDL",      reg(0x2181, 0x00,
                                    reg::access(reg::access::read | reg::access::write))},      //WRAM data low
                {"WMADDM",      reg(0x2182, 0x00,
                                    reg::access(reg::access::read | reg::access::write))},      //WRAM data mid
                {"WMADDH",      reg(0x2183, 0x00,
                                    reg::access(reg::access::read | reg::access::write))},      //WRAM data high

                {"NMITIMEN",    reg(0x4200, 0x00, reg::access::write)},      //counter enable
                {"WRIO",        reg(0x4201, 0x00, reg::access::write)},          //io port write
                {"WRMPYA",      reg(0x4202, 0x00, reg::access::write)},        //multiplicand A
                {"WRMPYB",      reg(0x4203, 0x00, reg::access::write)},       //multiplicand B
                {"WRDIVL",      reg(0x4204, 0x00, reg::access(
                        reg::access::write | reg::access::word))},       //divisor & dividend low
                {"WRDIVH",      reg(0x4205, 0x00, reg::access(
                        reg::access::write | reg::access::word))},       //divisor & dividend high
                {"WRDIVB",      reg(0x4206, 0x00, reg::access::write)},       //divisor & dividend B
                {"HTIMEL",      reg(0x4207, 0x00, reg::access(
                        reg::access::write | reg::access::word))},       //IRQ timer horizontal low
                {"HTIMEH",      reg(0x4208, 0x00, reg::access(
                        reg::access::write | reg::access::word))},       //IRQ timer horizontal high
                {"VTIMEL",      reg(0x4209, 0x00, reg::access(
                        reg::access::write | reg::access::word))},       //IRQ timer vertical low
                {"VTIMEH",      reg(0x420A, 0x00, reg::access(
                        reg::access::write | reg::access::word))},       //IRQ timer vertical high
                {"MDMAEN",      reg(0x420B, 0x00, reg::access::write)},       //DMA enable
                {"HDMAEN",      reg(0x420C, 0x00, reg::access::write)},       //HDMA enable
                {"MEMSEL",      reg(0x420D, 0x00, reg::access::write)},       //ROM Speed
                {"RDNMI",       reg(0x4210, 0x00)},                                   //NMI
                {"TIMEUP",      reg(0x4211, 0x00, reg::access(reg::access::read | reg::access::write))},     //Video IRQ
                {"HVBJOY",      reg(0x4212, 0x00,
                                    reg::access(reg::access::read | reg::access::write))},     //PPU Status
                {"RDIO",        reg(0x4213, 0x00)},                                    //IO Port Read
                {"RDDIVL",      reg(0x4214, 0x00, reg::access(
                        reg::access::read | reg::access::word))},                                  //divide result low
                {"RDDIVH",      reg(0x4215, 0x00, reg::access(
                        reg::access::read | reg::access::word))},                                  //divide result high
                {"RDMPYL",      reg(0x4216, 0x00, reg::access(
                        reg::access::read | reg::access::word))},                                  //multiply result low
                {"RDMPYH",      reg(0x4217, 0x00, reg::access(reg::access::read |
                                                              reg::access::word))},                                  //multiply result high
                {"JOY1L",       reg(0x4218, 0x00, reg::access(reg::access::read |
                                                              reg::access::word))},                                   //controller 1 data low
                {"JOY1H",       reg(0x4219, 0x00, reg::access(reg::access::read |
                                                              reg::access::word))},                                   //controller 1 data high
                {"JOY2L",       reg(0x421A, 0x00, reg::access(reg::access::read |
                                                              reg::access::word))},                                   //controller 2 data low
                {"JOY2H",       reg(0x421B, 0x00, reg::access(reg::access::read |
                                                              reg::access::word))},                                   //controller 2 data high
                {"JOY3L",       reg(0x421C, 0x00, reg::access(reg::access::read |
                                                              reg::access::word))},                                   //controller 3 data low
                {"JOY3H",       reg(0x421D, 0x00, reg::access(reg::access::read |
                                                              reg::access::word))},                                   //controller 3 data high
                {"JOY4L",       reg(0x421E, 0x00, reg::access(reg::access::read |
                                                              reg::access::word))},                                   //controller 4 data low
                {"JOY4H",       reg(0x421F, 0x00, reg::access(reg::access::read |
                                                              reg::access::word))},                                   //controller 4 data high

                {"DMAPX0",      reg(0x4300, 0x00, reg::access::write)},       //DMA 0 control
                {"BBADX0",      reg(0x4301, 0x00, reg::access::write)},       //DMA 0 destination
                {"A1TXL0",      reg(0x4302, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 0 source address
                {"A1TXH0",      reg(0x4303, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 0 source address
                {"A1BX0",       reg(0x4304, 0x00, reg::access::write)},       //DMA 0 source bank address
                {"DASXL0",      reg(0x4305, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 0 transfer size
                {"DASXH0",      reg(0x4306, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 0 & HDMA address
                {"NTRLX0",      reg(0x430A, 0x00, reg::access::write)},       //DMA 0 Number of lines for HDMA transfer

                {"DMAPX1",      reg(0x4310, 0x00, reg::access::write)},       //DMA 1 control
                {"BBADX1",      reg(0x4311, 0x00, reg::access::write)},       //DMA 1 destination
                {"A1TXL1",      reg(0x4312, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 1 source address
                {"A1TXH1",      reg(0x4313, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 1 source address
                {"A1BX1",       reg(0x4314, 0x00, reg::access::write)},       //DMA 1 source bank address
                {"DASXL1",      reg(0x4315, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 1 transfer size
                {"DASXH1",      reg(0x4316, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 1 & HDMA address
                {"NTRLX1",      reg(0x431A, 0x00, reg::access::write)},       //DMA 1 Number of lines for HDMA transfer

                {"DMAPX2",      reg(0x4320, 0x00, reg::access::write)},       //DMA 2 control
                {"BBADX2",      reg(0x4321, 0x00, reg::access::write)},       //DMA 2 destination
                {"A1TXL2",      reg(0x4322, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 2 source address
                {"A1TXH2",      reg(0x4323, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 2 source address
                {"A1BX2",       reg(0x4324, 0x00, reg::access::write)},       //DMA 2 source bank address
                {"DASXL2",      reg(0x4325, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 2 transfer size
                {"DASXH2",      reg(0x4326, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 2 & HDMA address
                {"NTRLX2",      reg(0x432A, 0x00, reg::access::write)},       //DMA 2 Number of lines for HDMA transfer

                {"DMAPX3",      reg(0x4330, 0x00, reg::access::write)},       //DMA 3 control
                {"BBADX3",      reg(0x4331, 0x00, reg::access::write)},       //DMA 3 destination
                {"A1TXL3",      reg(0x4332, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 3 source address
                {"A1TXH3",      reg(0x4333, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 3 source address
                {"A1BX3",       reg(0x4334, 0x00, reg::access::write)},       //DMA 3 source bank address
                {"DASXL3",      reg(0x4335, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 3 transfer size
                {"DASXH3",      reg(0x4336, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 3 & HDMA address
                {"NTRLX3",      reg(0x433A, 0x00, reg::access::write)},       //DMA 3 Number of lines for HDMA transfer

                {"DMAPX4",      reg(0x4340, 0x00, reg::access::write)},       //DMA 4 control
                {"BBADX4",      reg(0x4341, 0x00, reg::access::write)},       //DMA 4 destination
                {"A1TXL4",      reg(0x4342, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 4 source address
                {"A1TXH4",      reg(0x4343, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 4 source address
                {"A1BX4",       reg(0x4344, 0x00, reg::access::write)},       //DMA 4 source bank address
                {"DASXL4",      reg(0x4345, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 4 transfer size
                {"DASXH4",      reg(0x4346, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 4 & HDMA address
                {"NTRLX4",      reg(0x434A, 0x00, reg::access::write)},       //DMA 4 Number of lines for HDMA transfer

                {"DMAPX5",      reg(0x4350, 0x00, reg::access::write)},       //DMA 5 control
                {"BBADX5",      reg(0x4351, 0x00, reg::access::write)},       //DMA 5 destination
                {"A1TXL5",      reg(0x4352, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 5 source address
                {"A1TXH5",      reg(0x4353, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 5 source address
                {"A1BX5",       reg(0x4354, 0x00, reg::access::write)},       //DMA 5 source bank address
                {"DASXL5",      reg(0x4355, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 5 transfer size
                {"DASXH5",      reg(0x4356, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 5 & HDMA address
                {"NTRLX5",      reg(0x435A, 0x00, reg::access::write)},       //DMA 5 Number of lines for HDMA transfer

                {"DMAPX6",      reg(0x4360, 0x00, reg::access::write)},       //DMA 6 control
                {"BBADX6",      reg(0x4361, 0x00, reg::access::write)},       //DMA 6 destination
                {"A1TXL6",      reg(0x4362, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 6 source address
                {"A1TXH6",      reg(0x4363, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 6 source address
                {"A1BX6",       reg(0x4364, 0x00, reg::access::write)},       //DMA 6 source bank address
                {"DASXL6",      reg(0x4365, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 6 transfer size
                {"DASXH6",      reg(0x4366, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 6 & HDMA address
                {"NTRLX6",      reg(0x436A, 0x00, reg::access::write)},       //DMA 6 Number of lines for HDMA transfer

                {"DMAPX7",      reg(0x4370, 0x00, reg::access::write)},       //DMA 7 control
                {"BBADX7",      reg(0x4371, 0x00, reg::access::write)},       //DMA 7 destination
                {"A1TXL7",      reg(0x4372, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 7 source address
                {"A1TXH7",      reg(0x4373, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 7 source address
                {"A1BX7",       reg(0x4374, 0x00, reg::access::write)},       //DMA 7 source bank address
                {"DASXL7",      reg(0x4375, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 7 transfer size
                {"DASXH7",      reg(0x4376, 0x00,
                                    reg::access(reg::access::write | reg::access::word))},       //DMA 7 & HDMA address
                {"NTRLX7",      reg(0x437A, 0x00, reg::access::write)},       //DMA 7 Number of lines for HDMA transfer
        };

        this->m_cart_info = //additional cart info from ignoring fixed values : https://sneslab.net/wiki/SNES_ROM_Header and https://en.wikibooks.org/wiki/Super_NES_Programming/SNES_memory_map
                {
                        {"MAKER",        cart_data(0x7FB0, nullptr, 2)},  //Maker Code
                        {"GAME",         cart_data(0x7FB2, nullptr, 4)},  //Game Code
                        {"EXRAMSIZE",    cart_data(0x7FBD, nullptr, 1)},  //Expansion Ram Size
                        {"SVERSION",     cart_data(0x7FBE, nullptr, 1)},  //Special Version
                        {"CARTTYPE",     cart_data(0x7FBF, nullptr, 1)},  //Cartridge Type (Sub-Number)
                        {"TITLE",        cart_data(0x7FC0, nullptr, 21)},  //Cartridge title
                        {"MAPMODE",      cart_data(0x7FD5, nullptr, 1)},  //mapping mode
                        {"CARTINFO",     cart_data(0x7FD6, nullptr, 1)},  //ROM type
                        {"ROMSIZE",      cart_data(0x7FD7, nullptr, 1)},  //Rom size
                        {"RAMSIZE",      cart_data(0x7FD8, nullptr, 1)},  //SRam size
                        {"DEST",         cart_data(0x7FD9, nullptr, 1)},  //Destination code
                        {"VER",          cart_data(0x7FDB, nullptr, 1)},  //Masked Rom Version
                        {"CHECKSUMCOMP", cart_data(0x7FDC, nullptr, 2)},  //Checksum complement
                        {"CHECKSUM",     cart_data(0x7FDE, nullptr, 2)},  //Checksum
                };
    }

    void memoryController::init_ram() {

    }

    void memoryController::init_A_bus() {
        this->m_A_bus = new char8_t[3]; //24-bit
    }

    void memoryController::init_B_bus() {
        this->m_B_bus = new char8_t[1]; //8-bit
    }

    void memoryController::init_data_bus() {
        this->m_data_bus = new char8_t[1]; //8-bit
    }

    void memoryController::init_PPU_ram() {

    };

    void memoryController::setup_virtual_memory() { //http://www.emulatronia.com/doctec/consolas/snes/SNESMem.txt
        this->m_virtual_memory = new char8_t[0x1000000];
        int number_of_mappings = 64; //fixme: safe overestimate, this can and should be calculated/resized to not waste memory
        this->mapping = new ram_map[number_of_mappings];
        this->mapping[0] = ram_map(0x7E0000, 128 * 1024); //WRAM
        this->mapping[1] = ram_map(0x0000, 128 * 32); //WRAM mirror
        this->mapping[2] = ram_map(0x2100, 64); //PPU Registers
        this->mapping[3] = ram_map(0x4100, 64); //CPU Registers
        this->mapping[4] = ram_map(0x4016, 2); //Joypad mapping
        this->mapping[5] = ram_map(0x2140, 24256); //Aux
        //ROMSEL changes layout and mapping with different mappers, as such it wil be defined after rom load
    }

    bool memoryController::write_byte(char8_t *mem, int addr, char8_t data) {
        mem[addr] = data;
        return true;
    }

    bool memoryController::write_bytes(char8_t *mem, int start_addr, char8_t *data, int len) {
        for (int x = 0; x < len; x++)
            mem[start_addr + x] = data[x];
        return true;
    }

    bool memoryController::write_nulls(char8_t *mem, int start_addr, int len) {
        for (int x = 0; x < len; x++)
            mem[start_addr + x] = (char8_t) 0x00;
        return true;
    };

    bool memoryController::write_string(char8_t *mem, int start_addr, std::string data) {
        int len = data.length();
        auto *str_data = (char8_t *) data.c_str();
        return write_bytes(mem, start_addr, str_data, len);
    }


    std::pair<std::string, memoryController::reg>
    memoryController::Register_from_address(int addr) { //yes this is inefficient, but I either do this or
        // I change the registers' data structure
        for (auto const&[key, val]: this->m_registers) {
            if (val.addr == addr) {
                return std::pair<std::string, reg>(key, val);
            }
        }
        return {};
    }

    void memoryController::get_cart_info() {
        for (auto const&[key, val]: this->m_cart_info) {
            char *buf = new char[val.size];
            for (int x = 0; x < val.size; x++) {
                buf[x] = this->game_cart[val.addr + x];
            }
            this->m_cart_info[key].data = buf;
        }
    }

    void memoryController::update_registers() {
        for (auto const&[key, val]: this->m_registers) {
            this->m_registers[key].data = this->m_virtual_memory[val.addr];
        }
    }

    bool memoryController::load_rom_into_virtual_memory(char *rom, long int size) {
        if (size % 1024 == 0) {
            this->game_cart = reinterpret_cast<char8_t *>(rom);
        } else if (size % 1024 == 512) {
            this->game_cart = reinterpret_cast<char8_t *>(rom + 512);
        } else {
            printf("Malformed Rom Header!");
            return false;
        }

        this->get_cart_info();

        //Todo: mappers
        char map_mode = this->game_cart[this->m_cart_info["MAPMODE"].addr];
        bool fast_rom = (map_mode & 0x10) == 0x10;
        switch (map_mode) { //map modes from https://en.wikibooks.org/wiki/Super_NES_Programming/SNES_memory_map#How_do_I_recognize_the_ROM_type
            case 0x20:  //LoROM
                LoRom();
                return false;
                break;
            default:
                printf("Mapping mode: %02X not implemented!!!\n", map_mode);
                return false;
        }

        return true;
    }

//      :                  bank page byte
//      VIRTUAL MEMORY SIZE: $FF FF FF
    void memoryController::LoRom() {
        int game_rom_size = 0x400 << this->game_cart[this->m_cart_info["ROMSIZE"].addr];
        int game_sram_size = 0x400 << this->game_cart[this->m_cart_info["RAMSIZE"].addr];
        this->mapping[6] = ram_map(0x8000, game_rom_size); //rom
        this->mapping[7] = ram_map(0x0000, game_sram_size); //sram
    }
}