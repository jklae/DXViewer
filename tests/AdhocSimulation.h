#pragma once
#include <iostream>
#include "Win32App.h" // This includes ISimulation.h

class AdhocSimulation : public ISimulation
{
public:
	AdhocSimulation();
	~AdhocSimulation();

	void iUpdate(double timestep) override;
	std::vector<Vertex> iGetVertice() override;
	std::vector<unsigned int> iGetIndice() override;
	std::vector<DirectX::XMFLOAT4> iGetColor() override;
};

