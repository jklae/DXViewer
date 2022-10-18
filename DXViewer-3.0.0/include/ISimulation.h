#pragma once

struct Vertex;
struct ConstantBuffer;
class DX12App;

class ISimulation abstract
{
public:
	// ############################## Simulation methods ###################################
	// Method to update the simulation.
	virtual void iUpdate() = 0;

	// Method to reset the simulation.
	// * constantBuffer : Constant buffer of DX12App. You can use this to reset the constant buffer.
	virtual void iResetSimulationState(std::vector<ConstantBuffer>& constantBuffer) = 0;
	// #####################################################################################


	// ################################## Mesh methods #####################################
	// Method to get vertices/indices.
	virtual std::vector<Vertex>& iGetVertices() = 0;
	virtual std::vector<unsigned int>& iGetIndices() = 0;

	// Method to get the vertex/index buffer size.
	virtual UINT iGetVertexBufferSize() = 0;
	virtual UINT iGetIndexBufferSize() = 0;

	// Method to get the number of objects.
	virtual DirectX::XMINT3 iGetObjectCount() = 0;

	// Method to get the size of a object.
	// If you want to vary the size, use world matrices.
	virtual DirectX::XMFLOAT3 iGetObjectSize() = 0;

	// Method to get the offset of the object's position.
	virtual DirectX::XMFLOAT3 iGetObjectPositionOffset() = 0;
	// #####################################################################################


	// ################################ DirectX methods ####################################
	// Method to create objects.
	// * constantBuffer : Constant buffer of DX12App. Use this to create a constant buffer per object.
	virtual void iCreateObject(std::vector<ConstantBuffer>& constantBuffer) = 0;

	// Method to update constant buffers.
	// * constantBuffer : Constant buffer of DX12App.
	// * i : Index of the constant buffer's vector.
	virtual void iUpdateConstantBuffer(std::vector<ConstantBuffer>& constantBuffer, int i) = 0;

	// Method for Drawing Call.
	// * mCommandList : Command list for DX12App.
	// * size : Size of the constant buffer's vector.
	// * indexCount : The number of indices.
	// * Index of the constant buffer's vector.
	virtual void iDraw(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& mCommandList, int size, UINT indexCount, int i) = 0;

	// Method to set DX12App.
	// * dxApp : DX12App. Use this for WndProc methods.
	virtual void iSetDXApp(DX12App* dxApp) = 0;

	// Method to get the size of constant buffer's vector.
	virtual UINT iGetConstantBufferSize() = 0;

	// Method to check the update flag.
	// If true, DX12App updates your simulation.
	// If false, DX12App do not update your simulation except for Drawing Call.
	virtual bool iIsUpdated() = 0;
	// #####################################################################################


	// ################################# WndProc methods ##################################
	// These methods is for the window processor of Win32App.
	virtual void iWMCreate(HWND hwnd, HINSTANCE hInstance) = 0;
	virtual void iWMCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance) = 0;
	virtual void iWMHScroll(HWND hwnd, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance) = 0;
	virtual void iWMTimer(HWND hwnd) = 0;
	virtual void iWMDestory(HWND hwnd) = 0;
	// #####################################################################################

	// Destructor
	virtual ~ISimulation() = 0 {};
};

