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

    __declspec(dllexport) bool initialize(HINSTANCE hInstance, DX12App* dxapp, ISimulation* sim);
    __declspec(dllexport) int run();
    __declspec(dllexport) void initDirectX(DX12App* dxapp, ISimulation* sim);

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
    int _swState[2] = { 1, 1 };
    bool _updateFlag = true;

    POINT _mLastMousePos;


    // Convenience overrides for handling mouse input.
    void _onMouseDown(WPARAM btnState, int x, int y);
    void _onMouseUp(WPARAM btnState, int x, int y);
    void _onMouseMove(WPARAM btnState, int x, int y);

    void _synchronizeWinPos(_WINDOW state);
    void _switchWinState(_WINDOW state);
};

