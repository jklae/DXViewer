#include "AdhocSimulation.h"

using namespace DirectX;
using namespace std;
using namespace DXViewer::util;

AdhocSimulation::AdhocSimulation()
{
}

AdhocSimulation::~AdhocSimulation()
{
}

#pragma region Implementation
// ################################## Implementation ####################################
// Simulation methods
void AdhocSimulation::iUpdate()
{
}

void AdhocSimulation::iResetSimulationState(std::vector<ConstantBuffer>& constantBuffer)
{
}


// Mesh methods
vector<Vertex>& AdhocSimulation::iGetVertices()
{
	_vertices =
	{
		//              Position                 Normal
		Vertex({ { -0.5f, -0.5f, 0.0f },  { 0.0f, 0.0f, 0.0f } }),
		Vertex({ { -0.5f, +0.5f, 0.0f },  { 0.0f, 0.0f, 0.0f } }),
		Vertex({ { +0.5f, +0.5f, 0.0f },  { 0.0f, 0.0f, 0.0f } }),
		Vertex({ { +0.5f, -0.5f, 0.0f },  { 0.0f, 0.0f, 0.0f } })
	};

	return _vertices;
}

vector<unsigned int>& AdhocSimulation::iGetIndices()
{
	_indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3
	};

	return _indices;
}

UINT AdhocSimulation::iGetVertexBufferSize()
{
	return 4;
}

UINT AdhocSimulation::iGetIndexBufferSize()
{
	return 6;
}


// DirectX methods
void AdhocSimulation::iCreateObject(std::vector<ConstantBuffer>& constantBuffer)
{
	struct ConstantBuffer objectCB;

	objectCB.world = transformMatrix(0.0f, 0.0f, 0.0f, 1.0f);
	objectCB.worldViewProj = transformMatrix(0.0f, 0.0f, 0.0f);
	objectCB.color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	constantBuffer.push_back(objectCB);
}

void AdhocSimulation::iUpdateConstantBuffer(std::vector<ConstantBuffer>& constantBuffer, int i)
{
}

void AdhocSimulation::iDraw(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& mCommandList, int size, UINT indexCount, int i)
{
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mCommandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void AdhocSimulation::iSetDXApp(DX12App* dxApp)
{
	_dxApp = dxApp;
}

UINT AdhocSimulation::iGetConstantBufferSize()
{
	return 1;
}

DirectX::XMINT3 AdhocSimulation::iGetObjectCount()
{
	return { 1, 1, 0 };
}

DirectX::XMFLOAT3 AdhocSimulation::iGetObjectSize()
{
	return { 1.0f, 1.0f, 0.0f };
}

DirectX::XMFLOAT3 AdhocSimulation::iGetObjectPositionOffset()
{
	return { 0.0f, 0.0f, 0.0f };
}

bool AdhocSimulation::iIsUpdated()
{
	return true;
}


void AdhocSimulation::iWMCreate(HWND hwnd, HINSTANCE hInstance)
{
}

void AdhocSimulation::iWMCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance)
{
}

void AdhocSimulation::iWMHScroll(HWND hwnd, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance)
{
}

void AdhocSimulation::iWMTimer(HWND hwnd)
{
}

void AdhocSimulation::iWMDestory(HWND hwnd)
{
}



// #######################################################################################
#pragma endregion