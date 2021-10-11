#pragma once

// Console window is displayed in debug mode.
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include "AdhocSimulation.h" // This includes Win32App.h


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    AdhocSimulation* fluidsim = new AdhocSimulation();

    DX12App* dxapp = new DX12App();
    dxapp->setProjectionType(PROJ::PERSPECTIVE);

    Win32App winApp(800, 800);
    winApp.initialize(hInstance, dxapp, fluidsim);

    return winApp.run();
}