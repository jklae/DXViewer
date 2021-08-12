#pragma once
#include "Win32App.h"
#include "AdhocSimulation.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    int width = 800;
    int height = 600;

    Win32App winApp(width, height);
    winApp.Initialize(hInstance);

    AdhocSimulation* adhocsim = new AdhocSimulation();

    DX12App* dxapp = new DX12App();
    dxapp->SetSimulation(adhocsim, 0.05);

    winApp.InitDirectX(dxapp);

    return winApp.Run();
}