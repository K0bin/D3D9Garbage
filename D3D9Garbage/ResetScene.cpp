#include "ResetScene.h"

ResetScene::ResetScene(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device)) {
}

void ResetScene::Render() {
	HRESULT hr;
	hr = this->device->BeginScene();
	D3DPRESENT_PARAMETERS params = device->PresentParameters;
	this->device->Reset(&params);
	hr = this->device->EndScene();
	this->device->LightEnable(0, true);
}