#pragma once

#include "D3D9Test.h"

class StretchRectTest : public D3D9Test {

public:
	StretchRectTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:
	dxvk::Com<IDirect3DSurface9> src;
	dxvk::Com<IDirect3DSurface9> dst;

};
