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
	virtual LRESULT iSubWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance, bool& updateFlag, DX12App* dxapp) = 0;

	virtual void iUpdateConstantBuffer(std::vector<ConstantBuffer>& constantBuffer, int i) = 0;
	virtual void iDraw(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& mCommandList, int size, UINT indexCount, int i) = 0;

	virtual int getI() = 0;
	virtual void setI(int i) = 0;

	virtual void setDrawFlag(FLAG flagType, bool flag) = 0;

	virtual bool getDrawFlag(FLAG flagType) = 0;

	virtual ~ISimulation() = 0 {};
};

