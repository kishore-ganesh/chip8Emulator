#include "chip8.h"
#include <SDL2/SDL.h>
#include<thread>
#define PIXEL_SIZE 16
int main(int argc, char *argv[])
{
    Chip8 chip8;
    chip8.initialize();
    chip8.loadGame(argv[1]);
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    int windowWidth = PIXEL_SIZE*64;
    int windowHeight = PIXEL_SIZE*32;
    window = SDL_CreateWindow("chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    screenSurface  = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
    SDL_UpdateWindowSurface(window);

    std::thread background_thread(&Chip8::inputCycle, &chip8);
    std::thread timer_thread(&Chip8::timerCycle, &chip8);
    
    while(true){
        SDL_Event e;
        // SDL_Delay(2);
    
        
        
        chip8.emulateCycle();
        if(chip8.shouldDraw())
        {
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
        for(int i=0; i<32; i++)
        {
            for(int j=0; j<64; j++)
            {
                if(chip8.gfx[i][j])
                {
                    SDL_Rect pixel;
                    pixel.x=j*PIXEL_SIZE;
                    pixel.y=i*PIXEL_SIZE;
                    pixel.w = PIXEL_SIZE;
                    pixel.h = PIXEL_SIZE;
                    SDL_FillRect(screenSurface, &pixel, SDL_MapRGB(screenSurface->format, 255, 255, 255));
                }
            }
        }

        chip8.setDraw(false);
        }

        
        
        SDL_UpdateWindowSurface(window);


    }

}