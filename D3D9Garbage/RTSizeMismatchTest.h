#pragma once

#include "D3D9Test.h"

class RTSizeMismatchTest : public D3D9Test {

public:
	RTSizeMismatchTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:
	dxvk::Com<IDirect3DSurface9> ds;
	dxvk::Com<IDirect3DSurface9> dsMS;
	dxvk::Com<IDirect3DSurface9> dsLarge;
	dxvk::Com<IDirect3DSurface9> dsSmall;
	dxvk::Com<IDirect3DSurface9> rt;
	dxvk::Com<IDirect3DSurface9> rtMS;
	dxvk::Com<IDirect3DSurface9> rt1;
	dxvk::Com<IDirect3DSurface9> rt2;
	dxvk::Com<IDirect3DSurface9> rtLarge;
	dxvk::Com<IDirect3DSurface9> rtSmall;
	dxvk::Com<IDirect3DSurface9> rtSmall1;
	dxvk::Com<IDirect3DSurface9> rtSmall2;
	dxvk::Com<IDirect3DSurface9> rtSmall3;

};
