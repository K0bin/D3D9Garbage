#include "StretchRectTest.h"

StretchRectTest::StretchRectTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device))
{
	HRESULT res;
	res = device->CreateDepthStencilSurface(1280, 720, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, false, &src, nullptr);
	res = device->CreateDepthStencilSurface(1280, 720, D3DFMT_D32F_LOCKABLE, D3DMULTISAMPLE_NONE, 0, true, &dst, nullptr); // Lockable + Discard fails
	res = device->CreateDepthStencilSurface(1280, 720, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, true, &dst, nullptr);
}

void StretchRectTest::Render()
{
	HRESULT res;
	RECT rect = { 0, 0, 1280, 720 };
	// Discard on either src or dst makes stretch rect fail
	res = device->StretchRect(src.ptr(), &rect, dst.ptr(), &rect, D3DTEXF_NONE);

	res = device->Present(nullptr, nullptr, nullptr, nullptr);
}
