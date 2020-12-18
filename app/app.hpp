#pragma once
#include "olc/olcPixelGameEngine.h"
#include "MCS4.hpp"

class App :
    public olc::PixelGameEngine
{
public:
    App() {
        sAppName = "Intel4004 Emulator";
    }
private:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

    void printROM();
    void printStack();

    mcs4::MCS4 emulator;
};
