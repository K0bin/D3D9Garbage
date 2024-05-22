#pragma once
#pragma once

#include <string_view>
#include <vector>

#include "D3D9Test.h"

class StateBlockVertexOffsetTest : public D3D9Test {
public:
	StateBlockVertexOffsetTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:
	dxvk::Com<IDirect3DStateBlock9> sb;
	dxvk::Com<IDirect3DVertexBuffer9> vb;
	dxvk::Com<IDirect3DVertexBuffer9> vb2;
	dxvk::Com<IDirect3DVertexDeclaration9> vd;
	dxvk::Com<IDirect3DVertexDeclaration9> vd2;
	dxvk::Com<IDirect3DVertexDeclaration9> vd3;

	uint64_t frame = 0;
};

