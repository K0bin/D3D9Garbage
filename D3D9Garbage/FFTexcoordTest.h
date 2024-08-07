#pragma once

#include <string_view>
#include <vector>

#include "D3D9Test.h"

class FFTexcoordTest : public D3D9Test {
public:
	FFTexcoordTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:
	dxvk::Com<IDirect3DVertexBuffer9> vb;
	dxvk::Com<IDirect3DIndexBuffer9> ib;
	dxvk::Com<IDirect3DVolumeTexture9> texture;
	dxvk::Com<IDirect3DPixelShader9> ps;
};

