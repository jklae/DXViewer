#pragma once


#include <cassert>
#include <WindowsX.h>
#include <typeinfo>
#include <iostream>

#include "DX12App.h"

class Win32App
{
private:
    static Win32App* instanceForProc;
    const int kWidth;
    const int kHeight;
    HWND mhMainWnd = nullptr; // Main window handle
    DX12App* dxApp;

    POINT mLastMousePos;

    // Convenience overrides for handling mouse input.
    void OnMouseDown(WPARAM btnState, int x, int y);
    void OnMouseUp(WPARAM btnState, int x, int y);
    void OnMouseMove(WPARAM btnState, int x, int y);

    void Update();
    void Draw();

public:
    __declspec(dllexport) Win32App(const int kWidth, const int KHeight);
    __declspec(dllexport) ~Win32App();

    static Win32App* GetinstanceForProc();
    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    __declspec(dllexport) bool Initialize(HINSTANCE hInstance);
    __declspec(dllexport) int Run();
    __declspec(dllexport) void InitDirectX(DX12App* dxapp);
};

