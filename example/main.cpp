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
    dxapp->setCameraProperties(
        PROJ::PERSPECTIVE, 
        0.0f,               // orthogonal distance
        2.0f, 0.0f, 0.0f);  // radius, theta, phi
    dxapp->setBackgroundColor(DirectX::Colors::LightSlateGray);

    Win32App winApp(500, 500);
    winApp.setWinName(L"Adhoc Simulation");
    winApp.initialize(hInstance, dxapp, fluidsim);

    return winApp.run();
}