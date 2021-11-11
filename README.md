# DXViewer
## Overview
DXViewer is a DirectX 12 framework for physics simulation. This viewer makes it easy to visualize your simulation.

## Usage
Use the contents in the DXViewer-x.x.x folder. The example files in the **example** folder show how to use them.

### ISimulation
ISimulation is an interface that makes your simulation class compatible with DXViewer. You should implement this interface using your simulation class. The following is the abstract methods that should be implemented.
* void iUpdate()
* void iResetSimulationState(std::vector<ConstantBuffer>& constantBuffer)
* std::vector<Vertex> iGetVertice()
* std::vector<unsigned int> iGetIndice()
* int iGetObjectCount()
* void iCreateObjectParticle(std::vector<ConstantBuffer>& constantBuffer)
* void iWMCreate(HWND hwnd, HINSTANCE hInstance)
* void iWMCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance, bool& updateFlag, DX12App* dxapp) 
* void iWMHScroll(HWND hwnd, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance, DX12App* dxapp)
* void iWMTimer(HWND hwnd)
* void iWMDestory(HWND hwnd)
* void iUpdateConstantBuffer(std::vector<ConstantBuffer>& constantBuffer, int i)
* void iDraw(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& mCommandList, int size, UINT indexCount, int i)

## Build
This repo was developed in the following environment:
* Windows 10 64-bit
* Microsoft Visual Studio 2019 on x64 platform (C++14)
* CMake 3.19.0

If you build with cmake, you will get library files (.lib, .dll) and shader files (.cso).

## Reference
* Frank Luna. 2016. _Introduction to 3D Game Programming with DirectX 12_. Mercury Learning and Information.
