#pragma once
#include <vector>

struct Vertex;

class ISimulation abstract
{
public:
	virtual void IUpdate(double timestep) = 0;
	virtual std::vector<Vertex> IGetVertice() = 0;
	virtual std::vector<unsigned int> IGetIndice() = 0;

	virtual ~ISimulation() = 0 {};
};

