#pragma once

#include <string_view>
#include <vector>

#include "D3D9Test.h"

struct Vec3 {
	float x;
	float y;
	float z;
};

struct Vertex {
	Vec3 position;
	uint32_t diffuse;
};

class FFTest : public D3D9Test {
public:
	FFTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:
	dxvk::Com<IDirect3DVertexBuffer9> vb;
	dxvk::Com<IDirect3DIndexBuffer9> ib;
};

