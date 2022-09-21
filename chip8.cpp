#include "include/chip8.h"
#include <string>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <bitset>
#include <SFML/Graphics.hpp>

void Chip8::LoadROM(std::string filename)
{
    std::ifstream rom(filename, std::ios::binary);
    std::size_t i = 0;

    while (rom.good())
    {
        std::int32_t c = rom.get();
        if (c > -1)
        {
            memory.at(PC + i++) = c & 0x00ff;
        }
    }

    rom.close();
}

void Chip8::Emulate()
{
    int pixelScale = 20;
    sf::RenderWindow window(sf::VideoMode(static_cast<std::uint32_t>(64 * pixelScale), static_cast<std::uint32_t>(32 * pixelScale)), "Chip-8", sf::Style::Close);
    window.clear();
    window.display();
    memory[0x1FF] = 0x1;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }

                keys.at(0x1) = event.key.code == sf::Keyboard::Num1;
                keys.at(0x2) = event.key.code == sf::Keyboard::Num2;
                keys.at(0x3) = event.key.code == sf::Keyboard::Num3;
                keys.at(0xc) = event.key.code == sf::Keyboard::Num4;
                keys.at(0x4) = event.key.code == sf::Keyboard::Q;
                keys.at(0x5) = event.key.code == sf::Keyboard::W;
                keys.at(0x6) = event.key.code == sf::Keyboard::E;
                keys.at(0xd) = event.key.code == sf::Keyboard::R;
                keys.at(0x7) = event.key.code == sf::Keyboard::A;
                keys.at(0x8) = event.key.code == sf::Keyboard::S;
                keys.at(0x9) = event.key.code == sf::Keyboard::D;
                keys.at(0xe) = event.key.code == sf::Keyboard::F;
                keys.at(0xa) = event.key.code == sf::Keyboard::Y;
                keys.at(0x0) = event.key.code == sf::Keyboard::X;
                keys.at(0xb) = event.key.code == sf::Keyboard::C;
                keys.at(0xf) = event.key.code == sf::Keyboard::V;
            }
            if (event.type == sf::Event::KeyReleased)
            {
                keys.at(0x1) = keys.at(0x1) && !(event.key.code == sf::Keyboard::Num1);
                keys.at(0x2) = keys.at(0x2) && !(event.key.code == sf::Keyboard::Num2);
                keys.at(0x3) = keys.at(0x3) && !(event.key.code == sf::Keyboard::Num3);
                keys.at(0xc) = keys.at(0xc) && !(event.key.code == sf::Keyboard::Num4);
                keys.at(0x4) = keys.at(0x4) && !(event.key.code == sf::Keyboard::Q);
                keys.at(0x5) = keys.at(0x5) && !(event.key.code == sf::Keyboard::W);
                keys.at(0x6) = keys.at(0x6) && !(event.key.code == sf::Keyboard::E);
                keys.at(0xd) = keys.at(0xd) && !(event.key.code == sf::Keyboard::R);
                keys.at(0x7) = keys.at(0x7) && !(event.key.code == sf::Keyboard::A);
                keys.at(0x8) = keys.at(0x8) && !(event.key.code == sf::Keyboard::S);
                keys.at(0x9) = keys.at(0x9) && !(event.key.code == sf::Keyboard::D);
                keys.at(0xe) = keys.at(0xe) && !(event.key.code == sf::Keyboard::F);
                keys.at(0xa) = keys.at(0xa) && !(event.key.code == sf::Keyboard::Y);
                keys.at(0x0) = keys.at(0x0) && !(event.key.code == sf::Keyboard::X);
                keys.at(0xb) = keys.at(0xb) && !(event.key.code == sf::Keyboard::C);
                keys.at(0xf) = keys.at(0xf) && !(event.key.code == sf::Keyboard::V);
            }
        }

        for (std::uint8_t reg : v)
        {
            char hexString[20];
            sprintf(hexString, "%X", reg);
        }
        EmulateCycle();

        if (draw)
        {
            window.clear();
            int x = 0;
            int y = 0;
            for (std::uint8_t pixel : buffer)
            {
                sf::RectangleShape rectangle;
                rectangle.setSize(sf::Vector2f(pixelScale, pixelScale));
                rectangle.setFillColor(sf::Color::Black);
                if (pixel == 1)
                    rectangle.setFillColor(sf::Color::White);
                rectangle.setPosition(sf::Vector2f(x * pixelScale, y * pixelScale));
                window.draw(rectangle);

                ++x;
                if (x >= 64)
                {
                    x = 0;
                    ++y;
                }
            }
            window.display();

            draw = false;
        }
    }
}

void Chip8::EmulateCycle()
{

    uint16_t instruction = (memory[PC] << 8) | memory[PC + 1];
    std::uint8_t x = (instruction & 0x0f00) >> 8;
    std::uint8_t y = (instruction & 0x00f0) >> 4;
    std::uint16_t nnn = instruction & 0x0fff;
    std::uint8_t nn = instruction & 0x00ff;
    std::uint8_t n = instruction & 0x000f;
    PC += 2;
    // std::cout << std::bitset<16>(instruction).to_string() << std::endl;

    char hexString[20];
    sprintf(hexString, "%X", instruction & 0xF000);
    // std::cout << hexString << std::endl;
    for (bool k : keys)
        std::cout << k << std::endl;

    switch (instruction & 0xF000)
    {
    case 0x0000:
        switch (instruction)
        {
        case 0x00EE: // RET
            // --SP;
            // PC = stack[SP];
            PC = stack[stack.size() - 1];
            stack.erase(stack.end() - 1);
            break;

        case 0x00E0: // CLS
            // std::fill(buffer.begin(), buffer.end(), 0);
            for (int i = 0; i < buffer.size(); ++i)
                buffer[i] = 0;
            break;
        }
        break;

    case 0x1000: // JMP
        PC = nnn;
        break;

    case 0x2000: // CALL
        // ++SP;
        // stack[SP] = PC;
        stack.push_back(PC);
        PC = nnn;
        break;

    case 0x3000: // SKP VX == NN
        if (v.at(x) == nn)
            PC += 2;
        break;

    case 0x4000:
        if (v.at(x) != nn)
            PC += 2;
        break;

    case 0x5000:
        if (v.at(x) == v.at(y))
            PC += 2;
        break;

    case 0x6000: // SET
        v.at(x) = nn;
        break;

    case 0x7000: // ADD
        v[x] += nn;
        break;

    case 0x8000:
        switch (instruction & 0x000F)
        {
        case 0x0000: // SET
            v[x] = v[y];
            break;
        case 0x0001: // OR
            v[x] = v[x] | v[y];
            break;
        case 0x0002: // AND
            v[x] = v[x] & v[y];
            break;
        case 0x0003:
            v[x] = v[x] ^ v[y]; // XOR
            break;

        case 0x0004:
            if (v[x] + v[y] > 255) // ADD
                v[0xF] = 1;
            else
                v[0xF] = 0;

            v[x] = v[x] + v[y];
            break;

        case 0x0005: // SUB
            if (v[x] > v[y])
                v[0xF] = 1;
            else
                v[0xF] = 0;

            v[x] = v[x] - v[y];
            break;

        case 0x0006: // Right shift
            v[x] = v[y];
            v[0xF] = v[x] & 0x00000001;
            v[x] = v[x] >> 1;
            break;

        case 0x0007: /// SUB
            if (v[y] > v[x])
                v[0xF] = 1;
            else
                v[0xF] = 0;

            v[x] = v[y] - v[x];
            break;

        case 0x000E: // Left shift
            v[x] = v[y];
            v[0xF] = v[x] & 0x10000000;
            v[x] = v[x] << 1;
            break;
        }
        break;

    case 0x9000: // SNE
        if (v.at(x) != v.at(y))
            PC += 2;
        break;

    case 0xA000: // SET INDEX REGISTER
        I = nnn;
        break;

    case 0xD000: // DISPLAY
        DXYN(instruction);
        break;

    case 0xF000:
        switch (instruction & 0x00FF)
        {
        case 0x0033:
            break;

        case 0x0055:
            break;
        }
        break;
    }
}

void Chip8::DXYN(std::uint16_t instruction)
{
    std::uint8_t x = (instruction & 0x0f00) >> 8;
    std::uint8_t y = (instruction & 0x00f0) >> 4;
    std::uint8_t n = instruction & 0x000f;

    int width = 8;
    std::uint8_t xPos = v[x];
    std::uint8_t yPos = v[y];
    std::uint8_t pixel;
    v.at(0xF) = 0;
    draw = true;

    for (int row = 0; row < n; ++row)
    {
        pixel = memory[I + row];
        for (int col = 0; col < width; ++col)
        {
            if ((pixel & (0x80 >> col)) != 0)
            {
                // if sprite pixel == 1 && buffer (flip pixel)
                if (buffer[(xPos + col + ((yPos + row) * 64))] == 1)
                    v[0xF] = 1;
                buffer[(xPos + col + ((yPos + row) * 64))] ^= 1;
            }
        }
    }
}
