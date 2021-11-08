#pragma once

#ifndef DX12HEADER_H
#define DX12HEADER_H


// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <iostream>
#include <vector>
#include <memory>
#include <wrl.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXColors.h>

#include "d3dx12.h"

constexpr float EPS_FLOAT = 0.000001f;
constexpr float PI_FLOAT = 3.1415926535f;


struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 nor;
};


struct ConstantBuffer
{
	DirectX::XMFLOAT4X4 worldViewProj;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4 color;
};

enum class PROJ
{
	PERSPECTIVE,
	ORTHOGRAPHIC
};




// Matrix
inline DirectX::XMFLOAT4X4 __transformMatrix(
    const float x, const float y, const float z, 
    const float scale = 1.0f)
{
	// row major
    return DirectX::XMFLOAT4X4(
        scale, 0.0f, 0.0f, 0.0f,
        0.0f, scale, 0.0f, 0.0f,
        0.0f, 0.0f, scale, 0.0f,
        x, y, z, 1.0f);
}

template <typename T>
inline UINT __computeBufferByteSize()
{
	// Constant buffers must be a multiple of the minimum hardware
	// allocation size (usually 256 bytes).  So round up to nearest
	// multiple of 256.  We do this by adding 255 and then masking off
	// the lower 2 bytes which store all bits < 256.
	// Example: Suppose byteSize = 300.
	// (300 + 255) & ~255
	// 555 & ~255
	// 0x022B & ~0x00ff
	// 0x022B & 0xff00
	// 0x0200
	// 512
	return (sizeof(T) + 255) & ~255;
}



// ###################  Matrix multiplication operator overloading ###################
// row major
inline DirectX::XMFLOAT2 operator*(DirectX::XMFLOAT2 vec, DirectX::XMFLOAT4X4 mat)
{
	return 
	{ 
		(vec.x * mat._11) + (vec.y * mat._21), 
		(vec.x * mat._12) + (vec.y * mat._22) 
	};
}

//inline DirectX::XMFLOAT4X4 operator*(DirectX::XMFLOAT4X4 mat1, DirectX::XMFLOAT4X4 mat2)
//{
//	// Load
//	DirectX::XMMATRIX xmmatrix1 = XMLoadFloat4x4(&mat1);
//	DirectX::XMMATRIX xmmatrix2 = XMLoadFloat4x4(&mat2);
//
//	// Compute
//	DirectX::XMMATRIX resultMat = xmmatrix1 * xmmatrix2;
//
//	// Store
//	DirectX::XMFLOAT4X4 resultFloat4X4;
//	XMStoreFloat4x4(&resultFloat4X4, resultMat);
//
//	return resultFloat4X4;
//}




// ################### XMFLOAT2 operator ###################
inline float __length(DirectX::XMFLOAT2 f1)
{
	return sqrtf(f1.x * f1.x + f1.y * f1.y);
}

inline DirectX::XMFLOAT2 __fabsf(DirectX::XMFLOAT2 f1)
{
	return DirectX::XMFLOAT2(fabsf(f1.x), fabsf(f1.y));
}


// ################### XMFLOAT2 operator overloading ###################
// +
inline DirectX::XMFLOAT2 operator+(DirectX::XMFLOAT2 f1, DirectX::XMFLOAT2 f2)
{
	return DirectX::XMFLOAT2(f1.x + f2.x, f1.y + f2.y);
}

inline DirectX::XMFLOAT2 operator+(DirectX::XMFLOAT2 f1, float f2)
{
	return DirectX::XMFLOAT2(f1.x + f2, f1.y + f2);
}

// -
inline DirectX::XMFLOAT2 operator-(DirectX::XMFLOAT2 f1, DirectX::XMFLOAT2 f2)
{
	return DirectX::XMFLOAT2(f1.x - f2.x, f1.y - f2.y);
}

inline DirectX::XMFLOAT2 operator-(DirectX::XMFLOAT2 f1, float f2)
{
	return DirectX::XMFLOAT2(f1.x - f2, f1.y - f2);
}

// *
inline DirectX::XMFLOAT2 operator*(DirectX::XMFLOAT2 f1, float f2)
{
	return DirectX::XMFLOAT2(f1.x * f2, f1.y * f2);
}

// inner product
inline float operator*(DirectX::XMFLOAT2 f1, DirectX::XMFLOAT2 f2)
{
	return (f1.x * f2.x) + (f1.y * f2.y);
}

// /
inline DirectX::XMFLOAT2 operator/(DirectX::XMFLOAT2 f1, float f2)
{
	return DirectX::XMFLOAT2(f1.x / f2, f1.y / f2);
}

// +=
inline void operator+=(DirectX::XMFLOAT2& f1, DirectX::XMFLOAT2 f2)
{
	f1.x += f2.x;
	f1.y += f2.y;
}

inline void operator+=(DirectX::XMFLOAT2& f1, float f2)
{
	f1.x += f2;
	f1.y += f2;
}

// -=
inline void operator-=(DirectX::XMFLOAT2& f1, DirectX::XMFLOAT2 f2)
{
	f1.x -= f2.x;
	f1.y -= f2.y;
}

inline void operator-=(DirectX::XMFLOAT2& f1, float f2)
{
	f1.x -= f2;
	f1.y -= f2;
}



#endif