#pragma once

struct Vertex;
struct ConstantBuffer;
class DX12App;

class ISimulation abstract
{
public:
	// Simulation methods
	virtual void iUpdate() = 0;
	virtual void iResetSimulationState(std::vector<ConstantBuffer>& constantBuffer) = 0;

	// Mesh methods
	virtual std::vector<Vertex>& iGetVertice() = 0;
	virtual std::vector<unsigned int>& iGetIndice() = 0;
	virtual UINT iGetVertexBufferSize() = 0;
	virtual UINT iGetIndexBufferSize() = 0;

	// DirectX methods
	virtual void iCreateObject(std::vector<ConstantBuffer>& constantBuffer) = 0;
	virtual void iUpdateConstantBuffer(std::vector<ConstantBuffer>& constantBuffer, int i) = 0;
	virtual void iDraw(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& mCommandList, int size, UINT indexCount, int i) = 0;
	virtual void iSetDXApp(DX12App* dxApp) = 0;
	virtual UINT iGetConstantBufferSize() = 0;
	virtual DirectX::XMINT3 iGetDomainSize() = 0;

	// WndProc methods
	virtual void iWMCreate(HWND hwnd, HINSTANCE hInstance) = 0;
	virtual void iWMCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance) = 0;
	virtual void iWMHScroll(HWND hwnd, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance) = 0;
	virtual void iWMTimer(HWND hwnd) = 0;
	virtual void iWMDestory(HWND hwnd) = 0;

	// Win32 methods
	virtual bool iGetUpdateFlag() = 0;

	// Destructor
	virtual ~ISimulation() = 0 {};
};

