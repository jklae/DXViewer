#pragma once

#include "dx12header.h"


class DX12App
{
protected:

    struct ConstantBuffer
    {
        DirectX::XMFLOAT4X4 worldViewProj;
        DirectX::XMFLOAT4 color;
    };

    std::vector<ConstantBuffer> constantBuffer;
    std::vector<DirectX::XMFLOAT4X4> mWorld;
    
    std::vector<Vertex> vertices;
    std::vector<std::uint16_t> indices;

    const int kWidth;
    const int kHeight;

    const HWND mhMainWnd; // main window handle


#pragma region Init1
    // ######################################## Init 1 ##########################################
    // CheckMSAA
    // Set true to use 4X MSAA (4.1.8).  The default is false.
    bool m4xMsaaState = false;    // 4X MSAA enabled
    UINT m4xMsaaQuality = 0;      // quality level of 4X MSAA

    // CreateDevice
    Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

    // CreateFence
    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;

    // CreateCommandQueueAllocatorList
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

    // CreateSwapChain
    const DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    static const int SwapChainBufferCount = 2; // 'static' is required when declared in a class.
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;

    // CreateDescriptorHeap
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

    // CreateRTV
    UINT mRtvDescriptorSize;
    Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];

    // CreateDSV
    Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; // This is reused in pso.

    // SetViewport
    D3D12_VIEWPORT mScreenViewport;

    // SetScissorRectangle
    D3D12_RECT mScissorRect;

    void CheckMSAA();
    void CreateDevice();
    void CreateFence();
    void CreateCommandQueueAllocatorList();
    void CreateSwapChain();
    void CreateDescriptorHeap();
    void CreateRTV();
    void CreateDSV();
    void SetViewport();
    void SetScissorRectangle();

    // ##########################################################################################
#pragma endregion

#pragma region Init2
    // ######################################## Init 2 ##########################################

    // CreateVertexIndexBuffer
    Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
    BYTE* vMappedData = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;
    BYTE* iMappedData = nullptr;

    D3D12_VERTEX_BUFFER_VIEW vbv;
    D3D12_INDEX_BUFFER_VIEW ibv;
    UINT IndexCount = 0;


    // CreateConstantBuffer
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer = nullptr;
    BYTE* mMappedData = nullptr;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

    // CompileShader
    Microsoft::WRL::ComPtr<ID3DBlob> mvsByteCode = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> mpsByteCode = nullptr;
    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;


    DirectX::XMFLOAT4X4 mView = TransformMatrix(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT4X4 mProj = TransformMatrix(0.0f, 0.0f, 0.0f);


    void CreateProjMatrix();
    void CreateVertexIndexBuffer();
    //
    void CreateConstantBuffer();
    void CreateConstantBufferViewHeap();
    void CreateUploadBuffer();
    void CreateConstantBufferViews();
    void CreateRootSignature();
    //
    void CompileShader();
    void CreatePSO();

    // ##########################################################################################
#pragma endregion


#pragma region Draw
    // ########################################## Draw ##########################################
    UINT64 mCurrentFence = 0;
    int mCurrBackBuffer = 0;

    // CreatePSO (DirectXDrawingApp's)
    Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

    void FlushCommandQueue();
    void CloseCommandList();
    // ##########################################################################################
#pragma endregion


#pragma region Arcball
    // ####################################### Arcball ##########################################
    float mTheta = 1.5f * 3.14f;
    float mPhi = 3.14f / 2.0f;
    float mRadius = 5.0f;

    float Clamp(const float x, const float low, const float high);
    // ##########################################################################################
#pragma endregion

#pragma region Util
    // ######################################## Util ##########################################
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
    ID3D12Resource* CurrentBackBuffer() const;
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
    // ########################################################################################
#pragma endregion

public:
    DX12App(const int kWidth, const int kHeight, const HWND mhMainWnd);
    ~DX12App();
    DX12App(const DX12App& dxApp);

    void CreateObjects(const int count, const float scale);
    void SetVertexIndexResource(std::vector<Vertex> _vertices, std::vector<std::uint16_t> _indices);

    bool Initialize(const int count = 0, const float scale = 0.0f);

    void Update();
    void Draw();

    void UpdateVirtualSphereAngles(const POINT mLastMousePos, const int x, const int y);
    void UpdateVirtualSphereRadius(const POINT mLastMousePos, const int x, const int y);
};