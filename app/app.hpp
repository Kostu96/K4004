#pragma once
#include "app/third_party/olc/olcPixelGameEngine.h"
#include "emulator/source/MCS4.hpp"

class App :
    public olc::PixelGameEngine
{
public:
    App() {
        // TODO: do sth to cap fps
        sAppName = "Intel4004 Emulator";
    }
private:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

    void printROM();
    void printStack();

    MCS4 emulator;
};
