#include "AdhocSimulation.h"

using namespace DirectX;
using namespace std;

AdhocSimulation::AdhocSimulation()
{
}

AdhocSimulation::~AdhocSimulation()
{
}

void AdhocSimulation::iUpdate()
{
	//cout << "timestep : " << timestep << endl;
}

void AdhocSimulation::iResetSimulationState(std::vector<ConstantBuffer>& constantBuffer)
{
}

vector<Vertex> AdhocSimulation::iGetVertice()
{
	vector<Vertex> vertices =
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

	return vertices;
}

vector<unsigned int> AdhocSimulation::iGetIndice()
{
	vector<unsigned int> indices =
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

	return indices;
}

int AdhocSimulation::iGetObjectCount()
{
	return _objectCount;
}

void AdhocSimulation::iCreateObjectParticle(std::vector<ConstantBuffer>& constantBuffer)
{
	struct ConstantBuffer objectCB;
	// Multiply by a specific value to make a stripe
	objectCB.world = transformMatrix(0.0f, 0.0f, 0.0f, 0.8f);
	objectCB.worldViewProj = transformMatrix(0.0f, 0.0f, 0.0f);
	objectCB.color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	constantBuffer.push_back(objectCB);
}


void AdhocSimulation::iWMCreate(HWND hwnd, HINSTANCE hInstance)
{
}

void AdhocSimulation::iWMCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance, bool& updateFlag, DX12App* dxapp)
{
}

void AdhocSimulation::iWMHScroll(HWND hwnd, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance, DX12App* dxapp)
{
}

void AdhocSimulation::iWMTimer(HWND hwnd)
{
}

void AdhocSimulation::iWMDestory(HWND hwnd)
{
}



void AdhocSimulation::iUpdateConstantBuffer(std::vector<ConstantBuffer>& constantBuffer, int i)
{
}

void AdhocSimulation::iDraw(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& mCommandList, int size, UINT indexCount, int i)
{
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mCommandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
