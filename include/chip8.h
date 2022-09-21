#include <vector>
#include <cstdint>
#include <bitset>
#include <iostream>

class Chip8
{
public:
    void LoadROM(std::string);
    void Emulate();
    void EmulateCycle();

    Chip8()
    {
        v.resize(16);
        memory.resize(0x1000);
        stack.resize(16);
        flags.resize(8);
        buffer.resize(64 * 32);
        draw = false;

        for (std::size_t i = 0; i < memory.size(); i++)
            memory.at(i) = 0;

        for (std::size_t i = 0; i < FONT.size(); i++)
            memory.at(i) = FONT.at(i);

        PC = 0x200;
    }

private:
    std::uint16_t PC;
    std::uint16_t SP;
    std::uint16_t I; // MAR
    std::uint8_t dt;
    std::uint8_t st;
    std::vector<std::uint8_t> v;
    std::vector<std::uint8_t> memory;
    std::vector<std::uint8_t> buffer;
    std::vector<std::uint8_t> stack;
    std::vector<std::uint8_t> flags;
    std::vector<bool> keys;
    bool draw;

    void DXYN(std::uint16_t);

    std::vector<std::uint8_t> FONT = {
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
    };

    void Dump(int start, int stop)
    {
        for (int i = start; i < stop; ++i)
        {
            std::cout << "#" << i << ": " << std::bitset<8>(memory[i]).to_string() << std::endl;
        }
    };
};