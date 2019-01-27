# chip8Emulator
A C++ implementation of an  emulator for the CHIP8 System

This is a work in progress implementation of an emulator for the CHIP8 system. 34 opcodes have been implemented, and both graphics and input seem to be working fine(For most games).

Space Invaders is working correctly, as is Tetris. There are bugs in Pong, which we're working on.


SDL has been used for graphics, input and introducing timed delays.

Extensive testing has not been conducted yet, but it is rendering Pong and Space Invaders correctly.

Since 64*32 pixels is too small for modern devices, we mapped these pixels to larger 16*16 rectangles, and it looks fine.

P.S: The fontset was taken from multigesture.net. Huge thanks to this website for extensively documenting much of the CHIP8 emulation process, giving a great start to this project.

Future todos:
1. Fix input issues (DONE)
2. Make timer clock rates independent from emulation clock rate  (DONE)

