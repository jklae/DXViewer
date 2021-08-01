#pragma once

// Console window is displayed in debug mode.
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

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
    std::unique_ptr<DX12App> dxApp = nullptr;

    POINT mLastMousePos;

    // Convenience overrides for handling mouse input.
    void OnMouseDown(WPARAM btnState, int x, int y);
    void OnMouseUp(WPARAM btnState, int x, int y);
    void OnMouseMove(WPARAM btnState, int x, int y);

    void Update();
    void Draw();

    std::vector<Vertex> vertices1 =
    {
        Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f) }),
        Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f) }),
        Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f) }),
        Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f) }),
        Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f) }),
        Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f) }),
        Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f) }),
        Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f) }) //, XMFLOAT4(Colors::Black)
    };

    std::vector<Vertex> vertices2 =
    {
        Vertex({ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f) }),
        Vertex({ DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f) }),
        Vertex({ DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f) }),
        Vertex({ DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f) }),
        Vertex({ DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f) }),
        Vertex({ DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f) }),
        Vertex({ DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f) }),
        Vertex({ DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f) }) //, XMFLOAT4(Colors::Black)
    };

    std::vector<std::uint16_t> indices1 =
    {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 6, 5,
        4, 7, 6,

        // left face
        4, 5, 1,
        4, 1, 0,

        // right face
        3, 2, 6,
        3, 6, 7,

        // top face
        1, 5, 6,
        1, 6, 2,

        // bottom face
        4, 0, 3,
        4, 3, 7
    };

    std::vector<std::uint16_t> indices2 =
    {
        // left face
        4, 5, 1

    };

public:
    Win32App(const int kWidth, const int KHeight);
    ~Win32App();

    static Win32App* GetinstanceForProc();
    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    bool Initialize(HINSTANCE hInstance);
	int Run();
    void InitDirectX(SubFluidSimulation* fluidsim, double timestep);

    HWND GetMhMainWnd();
};

