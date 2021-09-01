#pragma once


#include <cassert>
#include <WindowsX.h>
#include <typeinfo>
#include <iostream>

#include "DX12App.h"

class Win32App
{
public:
    __declspec(dllexport) Win32App(const int kWidth, const int KHeight);
    __declspec(dllexport) ~Win32App();

    __declspec(dllexport) bool initialize(HINSTANCE hInstance);
    __declspec(dllexport) int run();
    __declspec(dllexport) void initDirectX(DX12App* dxapp, std::vector<ISimulation*> sim);

    static Win32App* getinstanceForProc();
    LRESULT mainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT subWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


private:
    static Win32App* _instanceForProc;
    const int _kWidth;
    const int _kHeight;
    HWND _mhWnd[2] = { nullptr, nullptr }; // Main window handle
    HINSTANCE _hInstance;
    DX12App* _dxApp = nullptr;

    enum class _WINDOW { MAIN, SUB };
    enum class _COM 
    { 
        GRID_BTN, PARTICLE_BTN, VELOCITY_BTN, 
        PLAY, STOP, NEXTSTEP,
        STATE_GROUP, LIQUID_RADIO, GAS_RADIO,
        SOLVER_GROUP, EULERIAN_RADIO, PIC_RADIO, FLIP_RADIO
    };
    int _swState[2] = { 1, 1 };
    bool _updateFlag = true;

    std::vector<ISimulation*> _sim;
    int _simIndex = 0;
    int _solverIndex = 0;

    POINT _mLastMousePos;



    void _update();
    void _draw();

    // Convenience overrides for handling mouse input.
    void _onMouseDown(WPARAM btnState, int x, int y);
    void _onMouseUp(WPARAM btnState, int x, int y);
    void _onMouseMove(WPARAM btnState, int x, int y);

    void _synchronizeWinPos(_WINDOW state);
    void _switchWinState(_WINDOW state);
};

