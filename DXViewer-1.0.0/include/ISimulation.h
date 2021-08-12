#pragma once

struct Vertex;

class ISimulation abstract
{
public:
	virtual void iUpdate(double timestep) = 0;
	virtual std::vector<Vertex> iGetVertice() = 0;
	virtual std::vector<unsigned int> iGetIndice() = 0;
	virtual std::vector<DirectX::XMFLOAT4> iGetColor() = 0;

	virtual ~ISimulation() = 0 {};
};

