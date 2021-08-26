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
    __declspec(dllexport) void initDirectX(DX12App* dxapp);

    static Win32App* getinstanceForProc();
    LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    static Win32App* _instanceForProc;
    const int _kWidth;
    const int _kHeight;
    HWND _mhMainWnd[2] = { nullptr, nullptr }; // Main window handle
    DX12App* _dxApp = nullptr;

    POINT _mLastMousePos;

    // Convenience overrides for handling mouse input.
    void _onMouseDown(WPARAM btnState, int x, int y);
    void _onMouseUp(WPARAM btnState, int x, int y);
    void _onMouseMove(WPARAM btnState, int x, int y);

    void _update();
    void _draw();
};

