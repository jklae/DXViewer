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
	//cout << "timestep : " << timestep << endl;
}

void AdhocSimulation::iResetSimulationState(std::vector<ConstantBuffer>& constantBuffer)
{
}


// Mesh methods
vector<Vertex>& AdhocSimulation::iGetVertice()
{
	_vertices =
	{
		Vertex({ XMFLOAT3(-0.5f, -0.5f, 0.0f) }),
		Vertex({ XMFLOAT3(-0.5f, +0.5f, 0.0f) }),
		Vertex({ XMFLOAT3(+0.5f, +0.5f, 0.0f) }),
		Vertex({ XMFLOAT3(+0.5f, -0.5f, 0.0f) })

		//Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f) }),
		//Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f) }),
		//Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f) }),
		//Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f) }),
		//Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f) }),
		//Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f) }),
		//Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f) }),
		//Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f) }) //, XMFLOAT4(Colors::Black)
	};

	return _vertices;
}

vector<unsigned int>& AdhocSimulation::iGetIndice()
{
	_indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		//// back face
		//4, 6, 5,
		//4, 7, 6,

		//// left face
		//4, 5, 1,
		//4, 1, 0,

		//// right face
		//3, 2, 6,
		//3, 6, 7,

		//// top face
		//1, 5, 6,
		//1, 6, 2,

		//// bottom face
		//4, 0, 3,
		//4, 3, 7
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

e
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