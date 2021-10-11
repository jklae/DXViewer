#pragma once

struct Vertex;
struct ConstantBuffer;
class DX12App;

class ISimulation abstract
{
public:

	virtual void iUpdate() = 0;
	virtual void iResetSimulationState(std::vector<ConstantBuffer>& constantBuffer) = 0;

	virtual std::vector<Vertex> iGetVertice() = 0;
	virtual std::vector<unsigned int> iGetIndice() = 0;
	virtual int iGetObjectCount() = 0;

	virtual void iCreateObjectParticle(std::vector<ConstantBuffer>& constantBuffer) = 0;

	virtual void iWMCreate(HWND hwnd, HINSTANCE hInstance) = 0;
	virtual void iWMCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance, bool& updateFlag, DX12App* dxapp) = 0;
	virtual void iWMHScroll(HWND hwnd, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance, DX12App* dxapp) = 0;
	virtual void iWMTimer(HWND hwnd) = 0;
	virtual void iWMDestory(HWND hwnd) = 0;

	virtual void iUpdateConstantBuffer(std::vector<ConstantBuffer>& constantBuffer, int i) = 0;
	virtual void iDraw(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& mCommandList, int size, UINT indexCount, int i) = 0;

	virtual ~ISimulation() = 0 {};
};

