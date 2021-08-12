#pragma once

#include <fstream>
#include <iostream>

#include "dx12header.h"
#include "ISimulation.h"


class DX12App
{
public:
    __declspec(dllexport) DX12App();
    __declspec(dllexport) ~DX12App();

    __declspec(dllexport) void setSimulation(ISimulation* fluidsim, double timestep);
    void setWindow(const int kWidth, const int kHeight, HWND mhMainWnd);

    bool initialize();
    void update();
    void draw();

    void updateVirtualSphereAngles(const POINT mLastMousePos, const int x, const int y);
    void updateVirtualSphereRadius(const POINT mLastMousePos, const int x, const int y);

    void setObjectCountXYZ(const int xCount, const int yCount, const int zCount);
    void setObjectScale(const float scale);

private:
    // createObject()
    int _objectCount[3] = { 0, 0, 0 };
    float _objectScale = 0.0f;

    // setSimulation()
    ISimulation* _fluidsim = nullptr;
    double _timestep;

    struct ConstantBuffer
    {
        DirectX::XMFLOAT4X4 worldViewProj;
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 color;
    };

    std::vector<ConstantBuffer> _constantBuffer;

    int _kWidth;
    int _kHeight;

    HWND _mhMainWnd; // main window handle


#pragma region Init1
    // ######################################## Init 1 ##########################################
    // CheckMSAA
    // Set true to use 4X MSAA (4.1.8).  The default is false.
    bool _m4xMsaaState = false;    // 4X MSAA enabled
    UINT _m4xMsaaQuality = 0;      // quality level of 4X MSAA

    // CreateDevice
    Microsoft::WRL::ComPtr<IDXGIFactory4> _mdxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12Device> _md3dDevice;

    // CreateFence
    Microsoft::WRL::ComPtr<ID3D12Fence> _mFence;

    // CreateCommandQueueAllocatorList
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> _mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _mCommandList;

    // CreateSwapChain
    const DXGI_FORMAT _mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    static const int _swapChainBufferCount = 2; // 'static' is required when declared in a class.
    Microsoft::WRL::ComPtr<IDXGISwapChain> _mSwapChain;

    // CreateDescriptorHeap
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _mRtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _mDsvHeap;

    // CreateRTV
    UINT _mRtvDescriptorSize;
    Microsoft::WRL::ComPtr<ID3D12Resource> _mSwapChainBuffer[_swapChainBufferCount];

    // CreateDSV
    Microsoft::WRL::ComPtr<ID3D12Resource> _mDepthStencilBuffer;
    DXGI_FORMAT _mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; // This is reused in pso.

    // SetViewport
    D3D12_VIEWPORT _mScreenViewport;

    // SetScissorRectangle
    D3D12_RECT _mScissorRect;

    void _checkMSAA();
    void _createDevice();
    void _createFence();
    void _createCommandQueueAllocatorList();
    void _createSwapChain();
    void _createDescriptorHeap();
    void _createRTV();
    void _createDSV();
    void _setViewport();
    void _setScissorRectangle();

    // ##########################################################################################
#pragma endregion

#pragma region Init2
    // ######################################## Init 2 ##########################################

    // CreateVertexIndexBuffer
    Microsoft::WRL::ComPtr<ID3D12Resource> _vertexBufferUploader = nullptr;
    BYTE* _vMappedData = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> _indexBufferUploader = nullptr;
    BYTE* _iMappedData = nullptr;

    D3D12_VERTEX_BUFFER_VIEW _vbv;
    D3D12_INDEX_BUFFER_VIEW _ibv;
    UINT _indexCount = 0;


    // CreateConstantBuffer
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _mCbvHeap = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> _mUploadBuffer = nullptr;
    BYTE* _mMappedData = nullptr;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> _mRootSignature = nullptr;

    // CompileShader
    Microsoft::WRL::ComPtr<ID3DBlob> _mvsByteCode = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> _mpsByteCode = nullptr;
    std::vector<D3D12_INPUT_ELEMENT_DESC> _mInputLayout;


    DirectX::XMFLOAT4X4 _mView = transformMatrix(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT4X4 _mProj = transformMatrix(0.0f, 0.0f, 0.0f);


    void _createObjects();
    void _createProjMatrix();
    void _createVertexIndexBuffer();
    //
    void _createConstantBuffer();
    void _createConstantBufferViewHeap();
    void _createUploadBuffer();
    void _createConstantBufferViews();
    void _createRootSignature();
    //
    void _compileShader();
    void _createPSO();

    // ##########################################################################################
#pragma endregion


#pragma region Draw
    // ########################################## Draw ##########################################
    UINT64 _mCurrentFence = 0;
    int _mCurrBackBuffer = 0;

    // CreatePSO (DirectXDrawingApp's)
    Microsoft::WRL::ComPtr<ID3D12PipelineState> _mPSO = nullptr;

    void _flushCommandQueue();
    void _closeCommandList();
    // ##########################################################################################
#pragma endregion


#pragma region Arcball
    // ####################################### Arcball ##########################################
    float _mTheta = 1.5f * 3.14f;
    float _mPhi = 3.14f / 2.0f;
    float _mRadius = 5.0f;

    float _clamp(const float x, const float low, const float high);
    // ##########################################################################################
#pragma endregion

#pragma region Util
    // ######################################## Util ##########################################
    D3D12_CPU_DESCRIPTOR_HANDLE _depthStencilView() const;
    ID3D12Resource* _currentBackBuffer() const;
    D3D12_CPU_DESCRIPTOR_HANDLE _currentBackBufferView() const;
    // ########################################################################################
#pragma endregion

};