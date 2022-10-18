#pragma once

// Console window is displayed in debug mode.
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include "AdhocSimulation.h" // This includes Win32App.h


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    // Simulation init
    AdhocSimulation* fluidsim = new AdhocSimulation();

    // DirectX init
    DX12App* dxapp = new DX12App();
    dxapp->setCameraProperties(
        PROJ::PERSPECTIVE, 
        2.0f, 0.0f, 0.0f);  // radius, theta, phi
    /*dxapp->setCameraProperties(
        PROJ::ORTHOGRAPHIC,
        0.0032f,               // orthogonal distance
        2.0f, 0.0f, 0.0f);  // radius, theta, phi     */
    dxapp->setBackgroundColor(DirectX::Colors::LightSlateGray);
    dxapp->setLightPosition(1.0f, 1.0f, 1.0f);

    // Window init
    Win32App winApp(500, 500);
    winApp.setWinName(L"Adhoc Simulation");
    winApp.setWinOffset(400, 200);
    winApp.initialize(hInstance, dxapp, fluidsim);

    return winApp.run();
}