#pragma once

#include <string_view>
#include <vector>

#include "D3D9Test.h"

class NullStreamTest : public D3D9Test {
public:
	NullStreamTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:
	dxvk::Com<IDirect3DVertexBuffer9> vb;
	dxvk::Com<IDirect3DVertexBuffer9> vb2;
	dxvk::Com<IDirect3DVertexBuffer9> colorvb;
	dxvk::Com<IDirect3DIndexBuffer9> ib;
	dxvk::Com<IDirect3DIndexBuffer9> ib2;
	dxvk::Com<IDirect3DVertexShader9> vs;
	dxvk::Com<IDirect3DPixelShader9> ps;
	dxvk::Com<IDirect3DVertexShader9> singleStreamVS;
	dxvk::Com<IDirect3DPixelShader9> singleStreamPS;
	dxvk::Com<IDirect3DVertexDeclaration9> vertexDecl;
	dxvk::Com<IDirect3DSurface9> rt;

	uint64_t frame = 0;
};

