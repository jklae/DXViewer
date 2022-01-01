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
	DirectX::XMFLOAT4X4 transInvWorld;
	DirectX::XMFLOAT4 color;
};

enum class PROJ
{
	PERSPECTIVE,
	ORTHOGRAPHIC
};



namespace DXViewer
{

	namespace util
	{
		template <typename T>
		inline UINT computeBufferByteSize()
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

		// Matrix
		inline DirectX::XMFLOAT4X4 transformMatrix(
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
	}
	


	namespace xmfloat2
	{
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

		inline DirectX::XMFLOAT2 operator+(float f1, DirectX::XMFLOAT2 f2)
		{
			return DirectX::XMFLOAT2(f1 + f2.x, f1 + f2.y);
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

		inline DirectX::XMFLOAT2 operator*(float f1, DirectX::XMFLOAT2 f2)
		{
			return DirectX::XMFLOAT2(f1 * f2.x, f1 * f2.y);
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

	}

	namespace xmfloat3
	{
		// ################### XMFLOAT2 operator overloading ###################
		// +
		inline DirectX::XMFLOAT3 operator+(float f1, DirectX::XMFLOAT3 f2)
		{
			return DirectX::XMFLOAT3(f1 + f2.x, f1 + f2.y, f1 + f2.z);
		}

		inline DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 f1, DirectX::XMFLOAT3 f2)
		{
			return DirectX::XMFLOAT3(f1.x + f2.x, f1.y + f2.y, f1.z + f2.z);
		}

		// -
		inline DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 f1)
		{
			return DirectX::XMFLOAT3(-f1.x, -f1.y, -f1.z);
		}

		inline DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 f1, float f2)
		{
			return DirectX::XMFLOAT3(f1.x - f2, f1.y - f2, f1.z - f2);
		}

		// *
		inline DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 f1, float f2)
		{
			return DirectX::XMFLOAT3(f1.x * f2, f1.y * f2, f1.z * f2);
		}

		inline DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 f1, DirectX::XMFLOAT3 f2)
		{
			return DirectX::XMFLOAT3(f1.x * f2.x, f1.y * f2.y, f1.z * f2.z);
		}
	}

	namespace xmint2
	{
		// ################### XMINT2 math function ###################
		inline int max_element(DirectX::XMINT2 i1)
		{
			return i1.x > i1.y ? i1.x : i1.y;
		}

		// ################### XMINT2 casting ###################
		inline DirectX::XMFLOAT2 int2_to_float2(DirectX::XMINT2 i1)
		{
			return DirectX::XMFLOAT2(static_cast<float>(i1.x), static_cast<float>(i1.y));
		}

		// ################### XMINT2 operator overloading ###################
		// -
		inline DirectX::XMINT2 operator-(DirectX::XMINT2 i1, int i2)
		{
			return DirectX::XMINT2(i1.x - i2, i1.y - i2);
		}
	}

	namespace xmint3
	{
		// ################### XMINT3 math function ###################
		inline int max_element(DirectX::XMINT3 i1)
		{
			int temp = (i1.x > i1.y ? i1.x : i1.y);
			return temp > i1.z ? temp : i1.z;
		}

		// ################### XMINT3 casting ###################
		inline DirectX::XMFLOAT3 int3_to_float3(DirectX::XMINT3 i1)
		{
			return DirectX::XMFLOAT3(
				static_cast<float>(i1.x), 
				static_cast<float>(i1.y),
				static_cast<float>(i1.z));
		}
	}
	
}

#endif