#include "LockableMSAATest.h"

LockableMSAATest::LockableMSAATest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device)) {
	HRESULT hr;

	dxvk::Com<IDirect3DDevice9Ex> deviceEx;
	hr = device->QueryInterface(__uuidof(IDirect3DDevice9Ex), reinterpret_cast<void**>(&deviceEx));

	dxvk::Com<IDirect3DSurface9> surface;
	hr = deviceEx->CreateRenderTargetEx(1024, 1024, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, true, &surface, nullptr, 0);
	//hr = deviceEx->CreateRenderTargetEx(1024, 1024, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONMASKABLE, 4, true, &surface, nullptr, 0);
	D3DLOCKED_RECT lockedRect;
	hr = surface->LockRect(&lockedRect, nullptr, 0);


	hr = deviceEx->CreateDepthStencilSurfaceEx(1024, 1024, D3DFMT_D16_LOCKABLE, D3DMULTISAMPLE_4_SAMPLES, 0, true, &surface, nullptr, 0);

	uint32_t tesT;

	hr = S_OK;

	/*D3DPRESENT_PARAMETERS params = {
		640, 480, D3DFMT_A8R8G8B8, 1, D3DMULTISAMPLE_4_SAMPLES, 0, D3DSWAPEFFECT_DISCARD, window, true, true, D3DFMT_D24S8, D3DPRESENTFLAG_LOCKABLE_BACKBUFFER, 0, D3DPRESENT_INTERVAL_DEFAULT
	};
	hr = device->Reset(&params);*/

	//device->CreateRenderTarget(1024, 1024, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT)
}

void LockableMSAATest::Render() {}