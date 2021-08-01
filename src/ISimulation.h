#pragma once

class ISimulation abstract
{
public:
	virtual void IUpdate(double timestep) = 0;
	virtual std::vector<float> IGetVertice() = 0;
};

