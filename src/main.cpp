#pragma once
#include "Win32App.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    int width = 800;
    int height = 600;

    Win32App winApp(width, height);
    winApp.Initialize(hInstance);


    winApp.InitDirectX();
    winApp.CreateObjects(50, 0.02f);

    return winApp.Run();
}