#pragma once
#include <iostream>
#include "ISimulation.h"
#include "dx12header.h"



class AdhocSimulation : public ISimulation
{
public:
	AdhocSimulation();
	~AdhocSimulation();

	void IUpdate(double timestep) override;
	std::vector<Vertex> IGetVertice() override;
	std::vector<unsigned int> IGetIndice() override;
};

