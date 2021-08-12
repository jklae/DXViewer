#pragma once
#include <iostream>
#include "Win32App.h" // This includes ISimulation.h

class AdhocSimulation : public ISimulation
{
public:
	AdhocSimulation();
	~AdhocSimulation();

	void IUpdate(double timestep) override;
	std::vector<Vertex> IGetVertice() override;
	std::vector<unsigned int> IGetIndice() override;
};

