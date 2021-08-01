#pragma once

class ISimulation abstract
{
public:
	virtual void IUpdate(double timestep) = 0;
	virtual std::vector<float> IGetVertice() = 0;
	virtual std::vector<unsigned int> IGetIndice() = 0;
};

