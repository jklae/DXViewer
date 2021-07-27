#pragma once

// Console window is displayed in debug mode.
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <cassert>
#include <WindowsX.h>
#include <typeinfo>

#include "DX12App.h"

class Win32App
{
private:
    static Win32App* instanceForProc;
    const int kWidth;
    const int kHeight;
    HWND mhMainWnd = nullptr; // Main window handle
    std::unique_ptr<DX12App> dxApp = nullptr;

    POINT mLastMousePos;

    // Convenience overrides for handling mouse input.
    void OnMouseDown(WPARAM btnState, int x, int y);
    void OnMouseUp(WPARAM btnState, int x, int y);
    void OnMouseMove(WPARAM btnState, int x, int y);

    void Update();
    void Draw();


public:
    Win32App(const int kWidth, const int KHeight);
    ~Win32App();

    static Win32App* GetinstanceForProc();
    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    bool Initialize(HINSTANCE hInstance);
	int Run();
    void InitDirectX();
    void CreateObjects(const int count, const float scale);

    HWND GetMhMainWnd();
};

