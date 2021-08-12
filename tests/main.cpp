#pragma once
#include "AdhocSimulation.h" // This includes Win32App.h


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    int width = 800;
    int height = 600;

    Win32App winApp(width, height);
    winApp.initialize(hInstance);

    AdhocSimulation* adhocsim = new AdhocSimulation();

    DX12App* dxapp = new DX12App();
    dxapp->setSimulation(adhocsim, 0.05);

    winApp.initDirectX(dxapp);

    return winApp.run();
}