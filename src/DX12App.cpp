#pragma once
#include "DX12App.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace std;
using namespace DXViewer::xmfloat3;
using namespace DXViewer::xmint3;

DX12App::DX12App()
{
}

DX12App::~DX12App()
{
	if (_md3dDevice != nullptr)
		_flushCommandQueue();

	// CreateVertexIndexBuffer
	_vMappedData = nullptr;
	_iMappedData = nullptr;

	if (_vertexBufferUploader != nullptr)
		_vertexBufferUploader->Unmap(0, nullptr);
	if (_indexBufferUploader != nullptr)
		_indexBufferUploader->Unmap(0, nullptr);

	// CreateConstantBuffer
	_mMappedData = nullptr;

	if (_mUploadBuffer != nullptr)
		_mUploadBuffer->Unmap(0, nullptr);

	delete _simulation;
}

void DX12App::setSimulation(ISimulation* simulation)
{
	_simulation = simulation;
}

void DX12App::setCameraProperties(PROJ proj, float orthoMRadius, float mRadius, float mTheta, float mPhi)
{
	_proj = proj;

	_orthoMRadius = orthoMRadius;
	_constMRadius = mRadius;
	_constMTheta = mTheta + (1.5f * PI_FLOAT);
	_constMPhi = mPhi + (PI_FLOAT / 2.0f);
}

void DX12App::setBackgroundColor(DirectX::XMVECTORF32 bgc)
{
	_backgroundColor = bgc;
}

void DX12App::setWindow(int kWidth, int kHeight, HWND mhMainWnd)
{
	_kWidth = kWidth;
	_kHeight = kHeight;
	_mhMainWnd = mhMainWnd;
}


bool DX12App::initialize()
{
	// Call after simulation init
	assert(_simulation != nullptr);

	// set angles, radius of VirtualSphere
	resetVirtualSphereAnglesRadius();

	// Init1
	_checkMSAA();
	_createDevice();
	_createFence();
	_createCommandQueueAllocatorList();
	_createSwapChain();
	_createDescriptorHeap();
	_createRTV();
	_createDSV();
	_setViewport();
	_setScissorRectangle();
		
    // Reset the command list to prep for initialization commands.
	_mCommandList->Reset(_mDirectCmdListAlloc.Get(), nullptr);

	// Init2
	_createObject();

	_createProjMatrix();
	_createVertexIndexBuffer();
	_createConstantBuffer();
	_compileShader();
	_createPSO();


	_closeCommandList();

	return true;
}

// ###### Adaptors of ISimuation ######
void DX12App::resetSimulationState()
{
	_simulation->iResetSimulationState(_constantBuffer);
}

void DX12App::wMCreate(HWND hwnd, HINSTANCE hInstance)
{
	_simulation->iWMCreate(hwnd, hInstance);
}

void DX12App::wMCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance)
{
	_simulation->iWMCommand(hwnd, msg, wParam, lParam, hInstance);
}

void DX12App::wMHScroll(HWND hwnd, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance)
{
	_simulation->iWMHScroll(hwnd, wParam, lParam, hInstance);
}

void DX12App::wMTimer(HWND hwnd)
{
	_simulation->iWMTimer(hwnd);
}

void DX12App::wMDestory(HWND hwnd)
{
	_simulation->iWMDestory(hwnd);
}

bool DX12App::isUpdated()
{
	return _simulation->iGetUpdateFlag();
}
// #####################################

#pragma region Init1
// ######################################## Init 1 ##########################################
void DX12App::_checkMSAA()
{
	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	// // but it doesn't work.... so m4xMsaaState is set to false.
	_m4xMsaaState = false;
	_m4xMsaaQuality = 0;
}

void DX12App::_createDevice()
{
	// DXGIFactory is used for swapchain interface creation and adapter enumeration.
	CreateDXGIFactory1(IID_PPV_ARGS(&_mdxgiFactory));

	// Try to create hardware device.		 // nullptr is default adapter
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_md3dDevice));
}

void DX12App::_createFence()
{
	// Fence
	_md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_mFence));
}

void DX12App::_createCommandQueueAllocatorList()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	_md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_mCommandQueue));

	_md3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(_mDirectCmdListAlloc.GetAddressOf())); // output

	_md3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		_mDirectCmdListAlloc.Get(), // Associated command allocator
		nullptr,                   // Initial PipelineStateObject
		IID_PPV_ARGS(_mCommandList.GetAddressOf())); // output

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	_mCommandList->Close();
}

void DX12App::_createSwapChain()
{
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = _kWidth;
	sd.BufferDesc.Height = _kHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = _mBackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = _m4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = _m4xMsaaState ? (_m4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = _swapChainBufferCount;
	sd.OutputWindow = _mhMainWnd;                               // windowsApp
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	_mdxgiFactory->CreateSwapChain(_mCommandQueue.Get(), &sd, _mSwapChain.GetAddressOf());
}

void DX12App::_createDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = _swapChainBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	_md3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(_mRtvHeap.GetAddressOf()));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	_md3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(_mDsvHeap.GetAddressOf()));
}

void DX12App::_createRTV()
{
	_mRtvDescriptorSize = _md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV); // RTV
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(_mRtvHeap->GetCPUDescriptorHandleForHeapStart());

	for (UINT i = 0; i < _swapChainBufferCount; i++)
	{
		_mSwapChain->GetBuffer(i, IID_PPV_ARGS(&_mSwapChainBuffer[i]));
		_md3dDevice->CreateRenderTargetView(_mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, _mRtvDescriptorSize);
	}
}

void DX12App::_createDSV()
{
	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = _kWidth;
	depthStencilDesc.Height = _kHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = _mDepthStencilFormat;

	depthStencilDesc.SampleDesc.Count = _m4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = _m4xMsaaState ? (_m4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	_md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, 
		&depthStencilDesc, D3D12_RESOURCE_STATE_COMMON, 
		nullptr, IID_PPV_ARGS(_mDepthStencilBuffer.GetAddressOf()));

	// Create descriptor to mip level 0 of entire resource using the format of the resource.
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHeapHandle(_depthStencilView());
	_md3dDevice->CreateDepthStencilView(_mDepthStencilBuffer.Get(), nullptr, dsvHeapHandle);

	// Transition the resource from its initial state to be used as a depth buffer.
	_mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(_mDepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
}

void DX12App::_setViewport()
{
	_mScreenViewport.TopLeftX = 0.0f;
	_mScreenViewport.TopLeftY = 0.0f;
	_mScreenViewport.Width = static_cast<float>(_kWidth);
	_mScreenViewport.Height = static_cast<float>(_kHeight);
	_mScreenViewport.MinDepth = 0.0f;
	_mScreenViewport.MaxDepth = 1.0f;
}

void DX12App::_setScissorRectangle()
{
	_mScissorRect = { 0, 0, _kWidth, _kHeight };
}
// ##########################################################################################
#pragma endregion

#pragma region Init2
// ######################################## Init 2 ##########################################
void DX12App::_createObject()
{
	_simulation->iCreateObject(_constantBuffer);
}

void DX12App::_createProjMatrix()
{
	XMMATRIX projMatrix;

	// Compute the projection matrix.
	switch (_proj)
	{
	case PROJ::PERSPECTIVE:
		projMatrix = XMMatrixPerspectiveFovLH(0.25f * 3.14f, static_cast<float>(_kWidth) / _kHeight, 1.0f, 1000.0f);
		break;
	case PROJ::ORTHOGRAPHIC:
	{
		// Compensate for normalized simulation coordinates.		
		projMatrix = XMMatrixOrthographicLH(_kWidth * _orthoMRadius, _kHeight * _orthoMRadius, 1.0f, 1000.0f);
	}
		break;
	default:
		projMatrix = XMMatrixIdentity();
		break;
	}
	
	XMStoreFloat4x4(&_mProj, projMatrix);
}

void DX12App::_createVertexIndexBuffer()
{
	// 2, 3
							//(UINT)vertices.size()
	const UINT vbByteSize = _simulation->iGetVertexBufferSize() * sizeof(Vertex);
							//(UINT)indices.size()
	const UINT ibByteSize = _simulation->iGetIndexBufferSize() * sizeof(unsigned int);

	_md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
		// &CD3DX12_RESOURCE_DESC::Buffer(vbByteSize)
		&CD3DX12_RESOURCE_DESC::Buffer(vbByteSize), D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(_vertexBufferUploader.GetAddressOf()));


	_vertexBufferUploader->Map(0, nullptr, reinterpret_cast<void**>(&_vMappedData));

	_vbv.BufferLocation = _vertexBufferUploader->GetGPUVirtualAddress();
	_vbv.StrideInBytes = sizeof(Vertex);
	_vbv.SizeInBytes = vbByteSize;


	_md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
		// &CD3DX12_RESOURCE_DESC::Buffer(ibByteSize)
		&CD3DX12_RESOURCE_DESC::Buffer(ibByteSize), D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(_indexBufferUploader.GetAddressOf()));

	_indexBufferUploader->Map(0, nullptr, reinterpret_cast<void**>(&_iMappedData));

	_ibv.BufferLocation = _indexBufferUploader->GetGPUVirtualAddress();
	_ibv.Format = DXGI_FORMAT_R32_UINT;
	_ibv.SizeInBytes = ibByteSize;

}

void DX12App::_createConstantBuffer()
{
	// 6
	_createConstantBufferViewHeap();
	_createUploadBuffer();
	_createConstantBufferViews();
	_createRootSignature();
}

void DX12App::_createConstantBufferViewHeap()
{
	// 6-4
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = _simulation->iGetConstantBufferSize();
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // The shader program will access this descriptor.
	cbvHeapDesc.NodeMask = 0;
	_md3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&_mCbvHeap));
}

void DX12App::_createUploadBuffer()
{
	UINT mElementByteSize = DXViewer::util::computeBufferByteSize<ConstantBuffer>();

	_md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * _simulation->iGetConstantBufferSize()), D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&_mUploadBuffer));

	_mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mMappedData));
}

void DX12App::_createConstantBufferViews()
{
	UINT objCBByteSize = DXViewer::util::computeBufferByteSize<ConstantBuffer>();
	for (int i = 0; i < _simulation->iGetConstantBufferSize(); i++)
	{
		//obj.CreateConstantBuffer(mCbvHeap, i, counts);
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_mCbvHeap->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(i, _md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = _mUploadBuffer->GetGPUVirtualAddress();
		cbAddress += i * objCBByteSize;

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbAddress;
		cbvDesc.SizeInBytes = objCBByteSize;

		_md3dDevice->CreateConstantBufferView(&cbvDesc, handle);
	}
}

void DX12App::_createRootSignature()
{
	// 6-5
	// Shader programs typically require resources as input (constant buffers,
	// textures, samplers).  The root signature defines the resources the shader
	// programs expect.  If we think of the shader programs as a function, and
	// the input resources as function parameters, then the root signature can be
	// thought of as defining the function signature.  

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[1];

	// Create a single descriptor table of CBVs.
	CD3DX12_DESCRIPTOR_RANGE cbvTable;
	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), 0);

	_md3dDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&_mRootSignature));
}

void DX12App::_compileShader()
{
	_mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// ..\\ext\\DXViewer\\shader\\vertexShader.hlsl
	HRESULT vertexShaderCompile = D3DCompileFromFile(L"..\\ext\\DXViewer\\shader\\vertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &_mvsByteCode, 0);
	HRESULT fragmentShaderCompile = D3DCompileFromFile(L"..\\ext\\DXViewer\\shader\\fragShader.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &_mpsByteCode, 0);
	assert(SUCCEEDED(vertexShaderCompile) && SUCCEEDED(fragmentShaderCompile));
	
	/*std::ifstream fin("vertexShader.cso", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	std::ifstream::pos_type size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);

	ComPtr<ID3DBlob> blob;
	D3DCreateBlob(size, blob.GetAddressOf());

	fin.read((char*)blob->GetBufferPointer(), size);
	fin.close();

	mvsByteCode = blob;

	std::ifstream fin2("fragShader.cso", std::ios::binary);

	fin2.seekg(0, std::ios_base::end);
	std::ifstream::pos_type size2 = (int)fin2.tellg();
	fin2.seekg(0, std::ios_base::beg);

	ComPtr<ID3DBlob> blob2;
	D3DCreateBlob(size2, blob2.GetAddressOf());

	fin2.read((char*)blob2->GetBufferPointer(), size2);
	fin2.close();

	mpsByteCode = blob2;*/
}

void DX12App::_createPSO()
{
	// 8, 9
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { _mInputLayout.data(), (UINT)_mInputLayout.size() };
	psoDesc.pRootSignature = _mRootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(_mvsByteCode->GetBufferPointer()), _mvsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(_mpsByteCode->GetBufferPointer()), _mpsByteCode->GetBufferSize()
	};
	// 8
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = _mBackBufferFormat;
	psoDesc.SampleDesc.Count = _m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = _m4xMsaaState ? (_m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = _mDepthStencilFormat;
	_md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&_mPSO));
}
// ##########################################################################################
#pragma endregion

#pragma region Draw
// ########################################## Draw ##########################################

void DX12App::_closeCommandList()
{
	// ############### 1. Close ################
	// Done recording commands.
	_mCommandList->Close();

	// ############### 2. Execute ################
	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { _mCommandList.Get() };
	_mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// ############### 3. Flush ################
	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	_flushCommandQueue();

	// ############### 4. Reset ################
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	_mDirectCmdListAlloc->Reset();

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	_mCommandList->Reset(_mDirectCmdListAlloc.Get(), _mPSO.Get());

}




void DX12App::_flushCommandQueue()
{
	// Advance the fence value to mark commands up to this fence point.
	_mCurrentFence++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	_mCommandQueue->Signal(_mFence.Get(), _mCurrentFence);

	// Wait until the GPU has completed commands up to this fence point.
	if (_mFence->GetCompletedValue() < _mCurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		_mFence->SetEventOnCompletion(_mCurrentFence, eventHandle);

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}
// ##########################################################################################
#pragma endregion



void DX12App::update()
{
	// ######### Update Vertex, Index buffer
	// Change View size
	_simulation->iUpdate();

	vertices = _simulation->iGetVertice();
	indices = _simulation->iGetIndice();

	const UINT vbByteSize = static_cast<UINT>(vertices.size()) * sizeof(Vertex);
	_vbv.SizeInBytes = vbByteSize;
	const UINT ibByteSize = static_cast<UINT>(indices.size()) * sizeof(unsigned int);
	_ibv.SizeInBytes = ibByteSize;

	// Update mapping data
	memcpy(&_vMappedData[0], vertices.data(), sizeof(Vertex) * vertices.size());
	memcpy(&_iMappedData[0], indices.data(), sizeof(unsigned int) * indices.size());
	_indexCount = static_cast<UINT>(indices.size());
	// #########

	// ######### Update Constant Buffer
	// Compensate for normalized simulation coordinates.		
	XMFLOAT3 objectCount = int3_to_float3(_simulation->iGetObjectCount());
	XMFLOAT3 objectSize = _simulation->iGetObjectSize();
	XMFLOAT3 objectPosOffset = _simulation->iGetObjectPositionOffset();
	//                  offset        +      half of count          *    size(width)
	XMFLOAT3 offset = objectPosOffset + ((objectCount - 1.0f) * 0.5f) * objectSize;

	int maxElement = max_element(_simulation->iGetObjectCount());
	float scale = static_cast<float>(maxElement);

	// Convert Spherical to Cartesian coordinates.
	float x = scale * _mRadius * sinf(_mPhi) * cosf(_mTheta);
	float z = scale * _mRadius * sinf(_mPhi) * sinf(_mTheta);
	float y = scale * _mRadius * cosf(_mPhi);

	// Build the view matrix.
	XMVECTOR eye = XMVectorSet(x + offset.x, y + offset.y, z + offset.z, 1.0f);
	XMVECTOR at = XMVectorSet(offset.x, offset.y, offset.z, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(eye, at, up);
	XMStoreFloat4x4(&_mView, view);

	XMMATRIX proj = XMLoadFloat4x4(&_mProj);

	UINT mElementByteSize = DXViewer::util::computeBufferByteSize<ConstantBuffer>();

	int size = _constantBuffer.size();
	for (int i = 0; i < size; i++)
	{
		_simulation->iUpdateConstantBuffer(_constantBuffer, i);

		XMMATRIX world = XMLoadFloat4x4(&_constantBuffer[i].world);
		XMMATRIX worldViewProj = world * view * proj;

		// Update the constant buffer with the latest worldViewProj matrix.
		XMStoreFloat4x4(&_constantBuffer[i].worldViewProj, worldViewProj);
		memcpy(&_mMappedData[i * mElementByteSize], &_constantBuffer[i], sizeof(ConstantBuffer));
	}
	// #########
}

void DX12App::draw()
{
	_mCommandList->RSSetViewports(1, &_mScreenViewport);
	_mCommandList->RSSetScissorRects(1, &_mScissorRect);

	// Indicate a state transition on the resource usage.
	_mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(_currentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// Clear the back buffer and depth buffer.
	_mCommandList->ClearRenderTargetView(_currentBackBufferView(), _backgroundColor, 0, nullptr);
	_mCommandList->ClearDepthStencilView(_depthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	_mCommandList->OMSetRenderTargets(1, &_currentBackBufferView(), true, &_depthStencilView());


	// Drawing Call
	// ------
	_mCommandList->SetGraphicsRootSignature(_mRootSignature.Get());

	_mCommandList->IASetVertexBuffers(0, 1, &_vbv);
	_mCommandList->IASetIndexBuffer(&_ibv);

	ID3D12DescriptorHeap* descriptorHeaps[] = { _mCbvHeap.Get() };
	_mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	int size = _constantBuffer.size();
	for (int i = 0; i < size; i++)
	{
		auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(_mCbvHeap->GetGPUDescriptorHandleForHeapStart());
		cbvHandle.Offset(i, _md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

		_mCommandList->SetGraphicsRootDescriptorTable(0, cbvHandle);

		_simulation->iDraw(_mCommandList, size, _indexCount, i);
	}
	// ------



	// Indicate a state transition on the resource usage.
	_mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(_currentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Done recording commands.
	_closeCommandList();


	// swap the back and front buffers
	_mSwapChain->Present(0, 0);
	_mCurrBackBuffer = (_mCurrBackBuffer + 1) % _swapChainBufferCount;
}


inline int INDEX(int i, int j) { return (i + 64 * j); };


#pragma region Arcball
// ####################################### Arcball ##########################################
void DX12App::updateVirtualSphereAngles(const POINT mLastMousePos, const int x, const int y)
{
	// Make each pixel correspond to a quarter of a degree.
	float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
	float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

	// Update angles based on input to orbit camera around box.
	_mTheta -= dx;
	_mPhi -= dy;

	// Restrict the angle mPhi.
	_mPhi = _clamp(_mPhi, 0.1f, PI_FLOAT - 0.1f);
}

void DX12App::updateVirtualSphereRadius(const POINT mLastMousePos, const int x, const int y)
{
	// Make each pixel correspond to 0.005 unit in the scene.
	float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
	float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

	// Update the camera radius based on input.
	_mRadius += dx - dy;

	// Restrict the radius.
	//_mRadius = _clamp(_mRadius, 3.0f, 15.0f);
}

void DX12App::resetVirtualSphereAnglesRadius()
{
	_mRadius = _constMRadius;
	_mTheta = _constMTheta;
	_mPhi = _constMPhi;
}

float DX12App::_clamp(const float x, const float low, const float high)
{
	return x < low ? low : (x > high ? high : x);
}
// ##########################################################################################
#pragma endregion


#pragma region Util
// ######################################## Util ##########################################
D3D12_CPU_DESCRIPTOR_HANDLE DX12App::_depthStencilView() const
{
	return _mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}

ID3D12Resource* DX12App::_currentBackBuffer() const
{
	return _mSwapChainBuffer[_mCurrBackBuffer].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE DX12App::_currentBackBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		_mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		_mCurrBackBuffer,
		_mRtvDescriptorSize);
}
// ########################################################################################
#pragma endregion