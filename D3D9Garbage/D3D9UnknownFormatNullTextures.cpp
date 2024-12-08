#include "D3D9UnknownFormatNullTextures.h"

D3D9UnknownFormatNullTextures::D3D9UnknownFormatNullTextures(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device)) {
}

void D3D9UnknownFormatNullTextures::Render() {
#

	HRESULT res;
	dxvk::Com<IDirect3DSurface9> surf;
	res = this->device->CreateOffscreenPlainSurface(128, 128, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, &surf, nullptr);
	res = this->device->CreateOffscreenPlainSurface(128, 128, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, &surf, nullptr);
	res = this->device->CreateOffscreenPlainSurface(128, 128, D3DFMT_UNKNOWN, D3DPOOL_SYSTEMMEM, &surf, nullptr);

	dxvk::Com<IDirect3DTexture9> tex;
	res = this->device->CreateTexture(128, 128, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, &tex, nullptr);
	res = this->device->CreateTexture(128, 128, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, &tex, nullptr);
	res = this->device->CreateTexture(128, 128, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_SYSTEMMEM, &tex, nullptr);

	res = this->device->CreateRenderTarget(128, 128, D3DFMT_UNKNOWN, D3DMULTISAMPLE_NONE, 0, false, &surf, nullptr);

}