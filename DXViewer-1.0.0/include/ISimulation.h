#pragma once

struct Vertex;
struct ConstantBuffer;

class ISimulation abstract
{
public:

	virtual void iUpdate() = 0;
	virtual void iResetSimulationState(std::vector<ConstantBuffer>& constantBuffer) = 0;

	virtual std::vector<Vertex> iGetVertice() = 0;
	virtual std::vector<unsigned int> iGetIndice() = 0;
	virtual DirectX::XMFLOAT4 iGetColor(int i) = 0;
	virtual int iGetObjectCount() = 0;
	virtual DirectX::XMFLOAT2 iGetParticlePos(int i) = 0;

	virtual void iCreateObjectParticle(std::vector<ConstantBuffer>& constantBuffer) = 0;
	virtual void iSubWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

	virtual int getI() = 0;
	virtual void setI(int i) = 0;

	virtual ~ISimulation() = 0 {};
};

