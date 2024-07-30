#pragma once

#include "D3D9Test.h"

class BufferLocking : public D3D9Test {

public:
	BufferLocking(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:
	dxvk::Com<IDirect3DVertexShader9> vs;
	dxvk::Com<IDirect3DPixelShader9> ps;
	dxvk::Com<IDirect3DVertexBuffer9> vb;
	dxvk::Com<IDirect3DIndexBuffer9> ib;
	dxvk::Com<IDirect3DVertexDeclaration9> vertexDecl;

	uint64_t frame = 0;

};
