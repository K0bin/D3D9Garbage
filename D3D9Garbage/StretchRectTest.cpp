#include "StretchRectTest.h"

StretchRectTest::StretchRectTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device))
{
	IDirect3DDevice9Ex* deviceEx = static_cast<IDirect3DDevice9Ex*>(device.ptr());

	HRESULT res;
	res = device->CreateDepthStencilSurface(1280, 720, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, false, &src, nullptr);
	res = device->CreateDepthStencilSurface(1280, 720, D3DFMT_D32F_LOCKABLE, D3DMULTISAMPLE_NONE, 0, true, &dst, nullptr); // Lockable + Discard fails
	res = device->CreateDepthStencilSurface(1280, 720, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, true, &dst, nullptr);

	res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32_LOCKABLE);
	res = d3d9->CheckDepthStencilMatch(0, D3DDEVTYPE_HAL, D3DFMT_D32_LOCKABLE, D3DFMT_X8R8G8B8, D3DFMT_D24X8);
	res = deviceEx->CreateDepthStencilSurfaceEx(1280, 720, D3DFMT_D32F_LOCKABLE, D3DMULTISAMPLE_NONE, 0, false, &src, nullptr, 0);
	res = deviceEx->CreateDepthStencilSurfaceEx(1280, 720, D3DFMT_D32_LOCKABLE, D3DMULTISAMPLE_NONE, 0, false, &src, nullptr, 0);
	res = deviceEx->CreateDepthStencilSurfaceEx(1280, 720, D3DFMT_D32_LOCKABLE, D3DMULTISAMPLE_NONE, 0, false, &src, nullptr, D3DUSAGE_DEPTHSTENCIL);

	D3DLOCKED_RECT locked;
	res = src->LockRect(&locked, nullptr, 0);
	res = src->UnlockRect();

	dxvk::Com<IDirect3DSurface9> backbuffer;
	res = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	res = backbuffer->LockRect(&locked, nullptr, 0);
	res = backbuffer->UnlockRect();
}

void StretchRectTest::Render()
{
	HRESULT res;
	RECT rect = { 0, 0, 1280, 720 };
	// Discard on either src or dst makes stretch rect fail
	res = device->StretchRect(src.ptr(), &rect, dst.ptr(), &rect, D3DTEXF_NONE);

	res = device->Present(nullptr, nullptr, nullptr, nullptr);
}
