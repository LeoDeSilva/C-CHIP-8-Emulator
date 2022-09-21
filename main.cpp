#include <iostream>
#include "include/chip8.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Expected filename argument\n";
        return 1;
    }

    Chip8 chip8;
    chip8.LoadROM(argv[1]);
    chip8.Emulate();
}