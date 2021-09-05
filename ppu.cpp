//
// Created by john on 05/09/2021.
//
#include "ppu.h"


namespace ppu {
    PPU::PPU() = default;
    PPU::PPU(memory::memoryController** memoryController) {
        this->m_memory_controller = *memoryController;
    }
    PPU::~PPU() = default;

    std::thread PPU::run() {
        auto testy = this->m_memory_controller;
        auto vram = testy->VRam();
        std::thread runtime([testy, vram](){
            if(SDL_Init(SDL_INIT_VIDEO) < 0) {
                printf("Failed to initialize SDL2\nSDL2 Error: %s\n", SDL_GetError());
                return -1;
            }
            auto cart_info = testy->Cart_info();
            auto game_name = "SNEkS Emulatator: "+std::string(cart_info["TITLE"].data, cart_info["TITLE"].size);
            SDL_Window *window = SDL_CreateWindow(game_name.c_str(),
                                                  SDL_WINDOWPOS_CENTERED,
                                                  SDL_WINDOWPOS_CENTERED,
                                                  1024, 896,
                                                  0);

            if(!window)
            {
                printf("Failed to create window\nSDL2 Error: %s\n", SDL_GetError());
                return -1;
            }

            SDL_Surface *window_surface = SDL_GetWindowSurface(window);

            if(!window_surface)
            {
                printf("Failed to get the surface from the window\nSDL2 Error: %s\n", SDL_GetError());
                return -1;
            }

            SDL_UpdateWindowSurface(window);

            Uint32 rmask, gmask, bmask, amask;

            rmask = 0x000000ff;
            gmask = 0x0000ff00;
            bmask = 0x00ff0000;
            amask = 0xff000000;


            int depth, pitch;
            depth = 32;
            pitch = 4*128;


            //SDL_CreateRGBSurfaceWithFormat    fixme: use with format, and change format depending on video mode
            SDL_Surface* VRAM_Surface = SDL_CreateRGBSurfaceFrom((void*)vram, 16, 16, depth, pitch, rmask, gmask, bmask, amask);

            bool keep_window_open = true;

            SDL_BlitScaled(VRAM_Surface, nullptr, window_surface, nullptr);
            while (keep_window_open) {
                SDL_Event e;
                while (SDL_PollEvent(&e) > 0) {

                    switch (e.type) {
                        case SDL_QUIT:
                            keep_window_open = false;
                            break;
                    }

                    SDL_UpdateWindowSurface(window);
                }
            }
            //SDL_FreeSurface(VRAM_Surface);
            return 0;
        });

        return runtime;
    }
}
