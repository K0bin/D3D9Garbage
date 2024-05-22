#pragma once

#include <string_view>
#include <vector>

#include "D3D9Test.h"

class ATIATOC : public D3D9Test {
public:
	ATIATOC(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:
	dxvk::Com<IDirect3DVertexBuffer9> vb;
	dxvk::Com<IDirect3DVertexShader9> vs;
	dxvk::Com<IDirect3DPixelShader9> ps;
	dxvk::Com<IDirect3DVertexDeclaration9> vertexDecl;
	dxvk::Com<IDirect3DSurface9> rt;
};
