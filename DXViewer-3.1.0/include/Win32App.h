#pragma once


#include <cassert>
#include <WindowsX.h>
#include <typeinfo>

#include "DX12App.h"

class Win32App
{
public:
    __declspec(dllexport) Win32App(const int kWidth, const int KHeight);
    __declspec(dllexport) ~Win32App();

    __declspec(dllexport) void setWinName(LPCWSTR winName);
    __declspec(dllexport) void setWinOffset(int offsetX, int offsetY);
    __declspec(dllexport) bool initialize(HINSTANCE hInstance, DX12App* dxapp, ISimulation* sim);
    __declspec(dllexport) int run();
    __declspec(dllexport) void initDirectX(DX12App* dxapp, ISimulation* sim);

    static Win32App* getinstanceForProc();
    LRESULT mainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT subWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


private:
    static Win32App* _instanceForProc;
    int _kWidth, _kHeight;
    int _offsetX, _offsetY;
    HWND _mhWnd[2] = { nullptr, nullptr }; // Main window handle
    HINSTANCE _hInstance;
    DX12App* _dxApp = nullptr;

    enum class _WINDOW { MAIN, SUB };
    int _swState[2] = { 1, 1 };

    POINT _mLastMousePos;
    LPCWSTR _winName = L"Win32 App";


    // Convenience overrides for handling mouse input.
    void _onMouseDown(WPARAM btnState, int x, int y);
    void _onMouseUp(WPARAM btnState, int x, int y);
    void _onMouseMove(WPARAM btnState, int x, int y);

    void _synchronizeWinPos(_WINDOW state);
    void _switchWinState(_WINDOW state);
};

