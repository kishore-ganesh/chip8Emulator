

#include "chip8.h"
#include<mutex>
using namespace std;

std::mutex delay_mutex;
std::mutex sound_mutex;

    // int convertToDecimal(short x)
    // {
    //     int sum = 0;
    //     int power = 0;
    //     while(x!=0)
    //     {
    //         int digit = x%10;
    //         x/=10;
    //         sum+=(digit*pow(2, power));
    //         power++;
    //     }

    //     return sum;
    // }
    int Chip8::findSizeOfFile(FILE *fp)
    {

        fseek(fp, 0, SEEK_END);
        int sz = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        return sz;
    }
    void Chip8::loadGame(char *path)
    {
        FILE *fp = fopen(path, "r");
        int sz = findSizeOfFile(fp);
        char *buffer = (char *)malloc(sz);
        fread(buffer, 1, sz, fp);

        for (int i = 0; i < sz; i++)
        {
            memory[i + 512] = buffer[i];
            
            // cout<<"BUFFER IS: "<<buffer[i]<<endl;
        }

        cout<<buffer[0]<<endl;
    }

    void Chip8::clearScreen()
    {
        printf("CLEARING SCREEN\n");
        setDraw(true);
        for (int i = 0; i < (32); i++)
        {
            for(int j=0; j<64; j++)
            {
                gfx[i][j] = 0;
            }
            
        }
    }

    void Chip8::skipIfVXEqualsNN()
    {
        printf("SKIP IF VX EQUALS NN\n");
        short x = (opcode & 0x0F00 )>> 8;

        if (V[x] == (opcode & 0x00FF))
        {
            pc += 2;
        }
    }

    void Chip8::skipIfVXNotEqualsNN()
    {
        printf("SKIP IF VX NOT EQUALS NN\n");
        short x = (opcode & 0x0F00 )>> 8;

        if (V[x] != (opcode & 0x00FF))
        {
            pc += 2;
        }
    }

    void Chip8::skipIfVXEqualsVY()
    {
        printf("SKIP IF VX EQUALS VY\n");
        short x = (opcode & 0x0F00 )>> 8;
        short y = (opcode & 0x00F0) >> 4;
        if (V[x] == V[y])
        {
            pc += 2;
        }
    }

    void Chip8::setVXToNN()
    {
        printf("SET  VX TO NN\n");
        short x = (opcode & 0x0F00 )>> 8;
        V[x] = opcode & 0x00FF;
    }

    void Chip8::addNNToVX()
    {
        printf("ADD NN TO VX\n");
        short x = (opcode & 0x0F00 )>> 8;
        V[x] += (opcode & 0x00FF);
        //don't set carry flag?
    }

    void Chip8::handleZero(short opcode)
    {
        switch (opcode & 0x00FF)
        {
        case 0x00E0:
        {
            clearScreen();
            pc += 2;
            break;
        }

        case 0x00EE:
        {
            returnFromSubroutine();
            break;
        }
        }
    }

    void Chip8::jumpToAddress()
    {
        
        
        // stack[sp] = pc + 2;
        pc = opcode & 0x0FFF;
        // sp++;
        
        printf("PC JUMP IS: %u\n", pc);
    }

    void Chip8::callSubroutine()
    {
        printf("CALLING SUBROUTINE\n");
        stack[sp] = pc+2;
        pc = opcode & 0x0FFF;
        printf("PC is: %u\n", pc);
        //jump
        sp++;
    }

    void Chip8::returnFromSubroutine()
    {
        printf("RETURNING FROM SUBROUTINE\n");
        pc = stack[--sp];
        // pc=stack[sp];
    }

    void Chip8::handleOne(short opcode)
    {
        printf("HANDLE ONE\n");
        jumpToAddress();
    }

    void Chip8::handleTwo(short opcode)
    {
        printf("HANDLE TWO\n");
        callSubroutine();
    }

    void Chip8::handleThree(short opcode)
    {
        printf("HANDLE THREE\n");
        skipIfVXEqualsNN();
        pc += 2;
    }

    void Chip8::handleFour(short opcode)
    {
        
        skipIfVXNotEqualsNN();
        pc += 2;
    }

    void Chip8::handleFive(short opcode)
    {
        skipIfVXEqualsVY();
        pc += 2;
    }

    void Chip8::handleSix(short opcode)
    {
        setVXToNN();
        pc += 2;
    }

    void Chip8::handleSeven(short opcode)
    {
        addNNToVX();
        pc += 2;
    }

    void Chip8::setVXToVY()
    {
        printf("VX EQUAL TO VY");
        short x = (opcode & 0x0F00 )>> 8;
        short y = (opcode & 0x00F0) >> 4;
        V[x] = V[y];
    }

    void Chip8::setVXToVXOrVY()
    {
        printf("VX EQUALS VX OR VY\n");
        short x = (opcode & 0x0F00 )>> 8;
        short y = (opcode & 0x00F0) >> 4;
        V[x] = V[x] | V[y];
    }

    void Chip8::setVXToVXAndVY()
    {
        printf("VX EQUALS VX AND VY\n");
        short x = (opcode & 0x0F00 )>> 8;
        short y = (opcode & 0x00F0) >> 4;
        V[x] = V[x] & V[y];
    }

    void Chip8::setVXToVXXOrVY()
    {
        printf("VX EQUALS VX XOR VY\n");
        short x = (opcode & 0x0F00 )>> 8;
        short y = (opcode & 0x00F0) >> 4;
        V[x] = V[x] ^ V[y];
    }

    void Chip8::setVXToVXPlusVY()
    {
        printf("VX EQUALS VX PLUS VY\n");
        
        short x = (opcode & 0x0F00 )>> 8;
        short y = (opcode & 0x00F0) >> 4;
        if((unsigned int)V[x]+V[y]>0xFF)
        {
            V[15] = 1;
        }

        else{
            V[15] = 0;
        }
        V[x] += V[y];
    }

    void Chip8::setVXToVXMinusVY()
    {
        printf("VX EQUALS VX  MINUS VY\n");
        short x = (opcode & 0x0F00 )>> 8;
        short y = (opcode & 0x00F0) >> 4;

        if(V[x]<V[y])
        {
            V[15] = 0;
        }
        else{
            V[15] = 1;
        }
        V[x] -= V[y];
    }

    void Chip8::setVXToVYMinusVX()
    {
        printf("VX EQUALS VY MINUS VX\n");
        short x = (opcode & 0x0F00 )>> 8;
        short y = (opcode & 0x00F0) >> 4;
        if(V[y]<V[x])
        {
            V[15] = 0;
        }

        else{
            V[15] = 1;
        }
        V[x] = V[y] - V[x];
    }

    void Chip8::setVXToRightShiftVX()
    {
        printf("VX EQUALS RIGHT SHIFT VX\n");
        short x = (opcode & 0x0F00 )>> 8;
        V[15] = V[x] & 0x0001;
        V[x] = V[x] >> 1;
    }

    void Chip8::setVXToLeftShiftVX()
    {
        printf("VX EQUALS LEFT SHIFT VX\n");
        short x = (opcode & 0x0F00 )>> 8;
        V[15] = V[x] & 0x8000;
        V[x] = V[x] << 1;
    }

    void Chip8::skipIfVXNotEqualsYY()
    {
        printf("SKIP IF VX NOT EQUAL TO VY\n");
        short x = (opcode & 0x0F00 )>> 8;
        short y = (opcode & 0x00F0) >> 4;
        if (V[x] != V[y])
        {
            pc += 2;
        }
    }

    void Chip8::setIToNNN()
    {
        printf("SET I TO NNN\n");
        I = opcode & 0x0FFF;
    }

    void Chip8::jumpToNNNPlusV0()
    {
        printf("JUMP TO NNN PLUS V0\n");
        pc = V[0] + (opcode & 0x0FFF);
    }

    void Chip8::setVXToRandAndNNN()
    {
        short x = (opcode & 0x0F00 )>> 8;
        V[x] = (rand() % 256) & (opcode & 0x00FF);
    }

    void Chip8::drawAtVXVY(){
        
        unsigned short x = (opcode & 0x0F00)>>8;
        unsigned short y = (opcode &0x00F0)  >> 4;
        unsigned short n = opcode &0x000F;
        setDraw(true);
        printf("DRAWING, x: %u, y: %u, n: %u,I: %u\n, ", x, y, n, I);
        V[15] = 0;
        for(int i = 0; i<n; i++)
        {
            char row = memory[I+i];
            
            for(int j=0; j<8; j++)
            {
                if(gfx[V[y]+i][V[x]+7-j]&(row&0x0001)){
                    V[15] = 1;
                }
                gfx[V[y]+i][V[x]+7-j] ^= (row&0x0001);
                // printf("GFX: %u, Row: %u \n", gfx[x+i][7-j], row);                                        
                row=row>>1;
            }
            
        }

    }

    void Chip8::setIToSprite(){
        printf("SET I TO SPRITE %u\n", V[(opcode&0x0F00)>>8]);
        short x = (opcode & 0x0F00)>>8;
        I = V[x]*5;

    }

    void Chip8::handleEight(short opcode)
    {
        switch (opcode & 0x000F)
        {
        case 0x0000:
        {

            setVXToVY();
            pc += 2;
            break;
        }

        case 0x0001:
        {

            setVXToVXOrVY();
            pc += 2;
            break;
        }

        case 0x0002:
        {
            setVXToVXAndVY();
            pc += 2;
            break;
        }

        case 0x0003:
        {
            setVXToVXXOrVY();
            pc += 2;
            break;
        }

        case 0x0004:
        {
            setVXToVXPlusVY();
            pc += 2;
            break;
        }

        case 0x0005:
        {

            setVXToVXMinusVY();
            pc += 2;
            break;
        }

        case 0x0006:
        {
            setVXToRightShiftVX();
            pc += 2;
            //Least significant bit in VF
            break;
        }

        case 0x0007:
        {
            setVXToVYMinusVX();
            pc += 2;
            break;
        }

        case 0x000E:
        {
            setVXToLeftShiftVX();
            pc += 2;
            break;
        }
        }
    }

    void Chip8::handleNine(short opcode)
    {
        skipIfVXNotEqualsYY();
        pc += 2;
    }

    void Chip8::handleA(short opcode)
    {
        setIToNNN();
        pc += 2;
    }

    void Chip8::handleB(short opcode)
    {
        jumpToNNNPlusV0();
    
    }

    void Chip8::handleC(short opcode)
    {
        setVXToRandAndNNN();
        pc += 2;
    }

    void Chip8::handleD(short opcode)
    {
        drawAtVXVY();
        pc+=2;
    }

    void Chip8::setVXToDelay(){
        short x = (opcode & 0x0F00 )>> 8;
        V[x] = delay_timer;
    }

    void Chip8::setVXToKey(){
        SDL_Event e;
        printf("WAITING FOR KEY\n");
        short x = (opcode & 0x0F00 )>> 8;
        while(e.type!=SDL_KEYDOWN)
        {
            SDL_PollEvent(&e);
        }

        printf("KEYPRESSED: %u\n", keymap[e.key.keysym.scancode]);
        V[x] = keymap[e.key.keysym.scancode];

    }

    

    void Chip8::handleF(short opcode)
    {
        switch(opcode&0x00FF)
        {
            case 0x07: {
                setVXToDelay();
                break;
            }
            case 0x0A:{
                setVXToKey();
                break;
            }
            case 0x15:{
                setDelayToVX();
                break;
            }

            case 0x18:{
                setSoundToVX();
                break;
            }

            case 0x1E:{
                setIToVXPlusI();
                break;
            }

            case 0x29:{
                setIToSprite();
                break;
            }

            case 0x33:{
                storeBCDInI();
                break;
            }

            case 0x55:{
                storeV0toVX();
                break;
            }
            case 0x65:{
                fillV0ToVX();
                break;
            }
        }
        pc+=2;
    }

    void Chip8::setDelayToVX(){
        printf("SET DELAY TO VX\n");
        short x = (opcode & 0x0F00 )>> 8;
        setDelay(V[x]);
        
    }

    void Chip8::setSoundToVX(){
        printf("SET SOUND TO VX\n");
        short x = (opcode & 0x0F00 )>> 8;
        setSound(V[x]);
    }


    void Chip8::setIToVXPlusI(){
        
        printf("SET I TO VX PLUS I\n");
        short x = (opcode & 0x0F00 )>> 8;
        I = I + V[x];
    }

    void Chip8::storeBCDInI(){
         printf("STORING BCD IN I\n");
         short x = (opcode & 0x0F00 )>> 8;
         short t = V[x];
         memory[I+2]=t%10;
         t/=10;
         memory[I+1]=t%10;
         t/=10;
         memory[I]=t%10;
         t/=10;

         printf("T is: %u, I: %u, I+1: %u, I+2: %u", t, memory[I], memory[I+1], memory[I+2]);
    }

    void Chip8::storeV0toVX(){
        printf("STORING V0 TO VX\n");
        short x = (opcode & 0x0F00 )>> 8;
        for(int i=0; i<=x; i++)
        {
            memory[I+i]=V[i];
        }
    }

    void Chip8::fillV0ToVX(){
        printf("FILLING V0 TO VX\n");
        short x = (opcode & 0x0F00 )>> 8;
        for(int i=0; i<=x; i++)
        {
            V[i] = memory[I+i];
        }
    }

    void  Chip8::setDelay(unsigned char value)
    {
        delay_mutex.lock();
        delay_timer = value;
        delay_mutex.unlock();
    }

    void Chip8::setSound(unsigned char value)
    {
        sound_mutex.lock();
        sound_timer = value;
        sound_mutex.unlock();
    }

    void Chip8::initialize()
    {
        pc = 0x200;
        srand(time(NULL));
        opcode = 0;
        I = 0;
        sp = 0;
        unsigned char chip8_fontset[80]={
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F            

        }; //Fontset copied from multigesture.net


        for (int i = 0; i < 4096; i++)
        {
            memory[i] = 0;
        }

        for(int i=0; i<80; i++)
        {
            memory[i] = chip8_fontset[i];
        }

        clearScreen();
        for (int i = 0; i < 16; i++)
        {
            stack[i] = 0;
            V[i] = 0;
        }

        keymap[SDL_SCANCODE_1]=0x0;
        keymap[SDL_SCANCODE_2]=0x1;
        keymap[SDL_SCANCODE_3]=0x2;
        keymap[SDL_SCANCODE_Q]=0x3;
        keymap[SDL_SCANCODE_W]=0x4; 
        keymap[SDL_SCANCODE_E]=0x5;
        keymap[SDL_SCANCODE_A]=0x6;
        keymap[SDL_SCANCODE_S]=0x7;
        keymap[SDL_SCANCODE_D]=0x8;
        keymap[SDL_SCANCODE_Z]=0x9;
        keymap[SDL_SCANCODE_X]=0xA;
        keymap[SDL_SCANCODE_C]=0xB;
        keymap[SDL_SCANCODE_R]=0xC;
        keymap[SDL_SCANCODE_F]=0xD;
        keymap[SDL_SCANCODE_V]=0xE;
        keymap[SDL_SCANCODE_T]=0xF;



        //load fontset
    }

    void Chip8::skipIfKeyEqualsVX(){
        short x = (opcode & 0x0F00 )>> 8;
        printf("SKIP IF KEY EQUALS VX: %u\n", V[x]);
        if(key[V[x]]==1)
        {
            pc+=2;
        }

    }

    void Chip8::skipIfKeyNotEqualsVX(){
        short x = (opcode & 0x0F00 )>> 8;
        printf("SKIP IF KEY NOT EQUALS VX %u\n", V[x]);
        if(key[V[x]]!=1)
        {
            pc+=2;
        }
    }

    void Chip8::handleE(short opcode)
    {
        switch(opcode&0x00FF)
        {
            case 0x009E:{
                skipIfKeyEqualsVX();
                break;
            }

            case 0x00A1:{
                skipIfKeyNotEqualsVX();
                break;
            }
        }
        pc+=2;
    }

    //Collision, and timing

    void Chip8::inputCycle(){
        while(true)
        {
            SDL_Event e;
        while(SDL_PollEvent(&e)!=0)
        {
            switch(e.type)
            {
                case SDL_KEYDOWN: {
                    
                    if(keymap.find(e.key.keysym.scancode)!=keymap.end()){
                        printf("KEY IS: %u\n", keymap[e.key.keysym.scancode]);
                        key[keymap[e.key.keysym.scancode]] = 1;
                    }
                    break;
                }

                case SDL_KEYUP:{
                     if(keymap.find(e.key.keysym.scancode)!=keymap.end()){
                        key[keymap[e.key.keysym.scancode]] = 0;
                        printf("KEY UP: %u\n", keymap[e.key.keysym.scancode]);
                    }
                    break;
                }

                case SDL_QUIT: {
                    SDL_Quit();
                }
            }
        }
        }
        
    }

    void Chip8::emulateCycle()
    {
        
        
        //May have to change this

        
        
        SDL_Event e;
        while(SDL_PollEvent(&e)!=0)
        {
            switch(e.type)
            {
                case SDL_KEYDOWN: {
                    
                    if(keymap.find(e.key.keysym.scancode)!=keymap.end()){
                        printf("KEY IS: %u\n", keymap[e.key.keysym.scancode]);
                        key[keymap[e.key.keysym.scancode]] = 1;
                    }
                    break;
                }

                case SDL_KEYUP:{
                     if(keymap.find(e.key.keysym.scancode)!=keymap.end()){
                        key[keymap[e.key.keysym.scancode]] = 0;
                        printf("KEY UP: %u\n", keymap[e.key.keysym.scancode]);
                    }
                    break;
                }

                case SDL_QUIT: {
                    SDL_Quit();
                }
            }
        }
        opcode = memory[pc] << 8 | memory[pc + 1];
        // cout<<"OPCODE: "<<memory[pc]<<" "<<endl;
        // pc += 2;
        //increment pc
        switch (opcode & 0xF000)
        {
        case 0x0000:
        {
            handleZero(opcode);
            break;
        }

        case 0x1000:
        {
            handleOne(opcode);
            break;
        }
        case 0x2000:
        {
            handleTwo(opcode);
            break;
        }
        case 0x3000:
        {
            handleThree(opcode);
            break;
        }
        case 0x4000:
        {
            handleFour(opcode);
            break;
        }
        case 0x5000:
        {
            handleFive(opcode);
            break;
        }
        case 0x6000:
        {
            handleSix(opcode);
            break;
        }
        case 0x7000:
        {
            handleSeven(opcode);
            break;
        }
        case 0x8000:
        {
            handleEight(opcode);
            break;
        }
        case 0x9000:
        {
            handleNine(opcode);
            break;
        }

        case 0xA000:
        {
            handleA(opcode);
            break;
        }
        case 0xB000:
        {
            handleB(opcode);
            break;
        }

        case 0xC000:
        {
            handleC(opcode);
            break;
        }
        case 0xD000:
        {
            handleD(opcode);
            break;
        }
        case 0xE000:
        {
            handleE(opcode);
            printf("HANDLE E\n");
            
            break;
        }
        case 0xF000:
        {
            handleF(opcode);
            break;
        }
        }

        
    }

    void Chip8::timerCycle(){
        while(true)
        {
            SDL_Delay(1000/60);
            if (delay_timer > 0)
        {
            setDelay(delay_timer-1);
            
        }

        if (sound_timer > 0)
        {
            setSound(sound_timer-1);
        }
        }
        
    }

    bool Chip8::shouldDraw(){
        return drawFlag;
    }

    void Chip8::setDraw(bool value){
        drawFlag = value;
    }

    
