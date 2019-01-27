#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<time.h>
#include<map>
#include<SDL2/SDL.h>
#define CLOCKSPEED 1000

 class Chip8
    {
        unsigned short opcode;
        unsigned char memory[4096];
        unsigned char V[16];
        unsigned short I;
        unsigned short pc;
        unsigned char delay_timer;
        unsigned char sound_timer;
        unsigned short stack[16];
        unsigned short sp;
        unsigned char key[16];
        std::map<SDL_Keycode, short> keymap; 
        bool drawFlag;

      public:
        unsigned char gfx[32][64]; //Make this private later
        int findSizeOfFile(FILE *fp);
        void loadGame(char *path);
        void clearScreen();
        void skipIfVXEqualsNN();
        void skipIfVXNotEqualsNN();
        void skipIfVXEqualsVY();
        void skipIfKeyEqualsVX();
        void skipIfKeyNotEqualsVX();
        void setVXToNN();
        void addNNToVX();
        void handleZero(short opcode);
        void jumpToAddress();
        void callSubroutine();
        void returnFromSubroutine();
        void handleOne(short opcode);
        void handleTwo(short opcode);
        void handleThree(short opcode);
        void handleFour(short opcode);
        void handleFive(short opcode);
        void handleSix(short opcode);
        void handleSeven(short opcode);
        void setVXToVY();
        void setVXToVXOrVY();
        void setVXToVXAndVY();
        void setVXToVXXOrVY();
        void setVXToVXPlusVY();
        void setVXToVXMinusVY();
        void setVXToVYMinusVX();
        void setVXToRightShiftVX();
        void setVXToLeftShiftVX();
        void setVXToDelay();
        void setVXToKey();
        void skipIfVXNotEqualsYY();
        void setIToNNN();
        void jumpToNNNPlusV0();
        void setVXToRandAndNNN();
        void drawAtVXVY();
        void setIToSprite();
        void handleEight(short opcode);
        void handleNine(short opcode);
        void handleA(short opcode);
        void handleB(short opcode);
        void handleC(short opcode);
        void handleD(short opcode);
        void handleE(short opcode);
        void handleF(short opcode);
        void setDelayToVX();
        void setSoundToVX();
        void setIToVXPlusI();
        void storeBCDInI();
        void storeV0toVX();
        void fillV0ToVX();
        void initialize();
        void emulateCycle();
        bool shouldDraw();
        void setDraw(bool flag);
        void inputCycle();
        void setDelay(unsigned char value);
        void setSound(unsigned char value);
        void timerCycle();
        
    };



