#include "AdhocSimulation.h"

using namespace DirectX;
using namespace std;

AdhocSimulation::AdhocSimulation()
{
}

AdhocSimulation::~AdhocSimulation()
{
}

void AdhocSimulation::iUpdate(double timestep)
{
	//cout << "timestep : " << timestep << endl;
}

vector<Vertex> AdhocSimulation::iGetVertice()
{
	vector<Vertex> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f) }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f) }) //, XMFLOAT4(Colors::Black)
	};

	return vertices;
}

vector<unsigned int> AdhocSimulation::iGetIndice()
{
	vector<unsigned int> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	return indices;
}

vector<XMFLOAT4> AdhocSimulation::iGetColor()
{
	vector<XMFLOAT4> color;
	color.push_back(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	color.push_back(XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	color.push_back(XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));
	color.push_back(XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	color.push_back(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	return color;
}

int* AdhocSimulation::iGetObjectCountXYZ()
{
	return _objectCount;
}

float AdhocSimulation::iGetObjectScale()
{
	return _objectScale;
}

float AdhocSimulation::iGetObjectSize()
{
	return _objectSize;
}

void AdhocSimulation::iSetObjectCountXYZ(int xCount, int yCount, int zCount)
{
	// 0 is not allowed.
	assert((xCount != 0) && (yCount != 0) && (zCount != 0));

	_objectCount[0] = xCount;
	_objectCount[1] = yCount;
	_objectCount[2] = zCount;
}

void AdhocSimulation::iSetObjectScale(float objectScale)
{
	// 0 is not allowed.
	assert(objectScale != 0.0f);

	_objectScale = objectScale;
}
