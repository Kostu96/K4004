#pragma once
#include "app/third_party/olc/olcPixelGameEngine.h"
#include "emulator/source/MCS4.hpp"

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
    void printCPU();

    MCS4 emulator;
};
