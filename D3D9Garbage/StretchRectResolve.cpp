#include "StretchRectResolve.h"

#include <array>

#include <fstream>
#include <vector>

StretchRectResolve::StretchRectResolve(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device)) {
}

void StretchRectResolve::Render() {
	HRESULT res;

	std::array<Vertex, 3> quad = {
		-1.0f, -1.0f, 0.1f, 0xffff0000,
		-1.0f,  1.0f, 0.1f, 0xffff0000,
		 1.0f,  1.0f, 0.1f, 0xffff0000,
	};
	std::array<uint16_t, 3> indices = { 0, 1, 2 };

	//res = this->device->CreateVertexBuffer(sizeof(quad), D3DUSAGE_DYNAMIC, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &this->vb, nullptr);
	//res = this->device->CreateVertexBuffer(sizeof(quad), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &this->vb, nullptr);
	//res = this->device->CreateVertexBuffer(sizeof(quad), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &this->vb, nullptr);
	res = this->device->CreateVertexBuffer(sizeof(quad), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_SYSTEMMEM, &this->vb, nullptr);
	void* vbPtr = nullptr;
	this->vb->Lock(0, 0, &vbPtr, sizeof(quad));
	memcpy(vbPtr, quad.data(), sizeof(quad));
	this->vb->Unlock();

	void* ibPtr = nullptr;
	res = this->device->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &this->ib, nullptr);
	this->ib->Lock(0, 0, &ibPtr, 0);
	memcpy(ibPtr, indices.data(), sizeof(indices));
	this->ib->Unlock();

	dxvk::Com<IDirect3DSwapChain9> swapchain;
	res = this->device->GetSwapChain(0, &swapchain);
	D3DPRESENT_PARAMETERS presentParams;
	res = swapchain->GetPresentParameters(&presentParams);
	dxvk::Com<IDirect3DSurface9> backbuffer;
	res = swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	dxvk::Com<IDirect3DSurface9> rt;
	D3DMULTISAMPLE_TYPE samples = D3DMULTISAMPLE_8_SAMPLES;
	//samples = D3DMULTISAMPLE_NONE;
	res = this->device->CreateRenderTarget(32, 32, presentParams.BackBufferFormat, samples, 0, false, &rt, nullptr);
	res = this->device->SetRenderTarget(0, rt.ptr());

	res = this->device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	res = this->device->SetRenderState(D3DRS_LIGHTING, false);
	res = this->device->SetStreamSource(0, this->vb.ptr(), 0, sizeof(Vertex));
	res = this->device->SetIndices(this->ib.ptr());
	res = this->device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	res = this->device->SetRenderState(D3DRS_ZENABLE, false);
	res = this->device->Clear(0, nullptr, D3DCLEAR_TARGET, 0xff0000ff, 1.0f, 0);
	dxvk::Com<IDirect3DSurface9> ds;
	res = this->device->GetDepthStencilSurface(&ds);
	res = this->device->BeginScene();
	res = this->device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 1);
	res = this->device->EndScene();

	D3DSURFACE_DESC desc;
	res = rt->GetDesc(&desc);

	/*dxvk::Com<IDirect3DSurface9> offscreen;
	res = this->device->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &offscreen, nullptr);
	res = this->device->GetRenderTargetData(rt.ptr(), offscreen.ptr());
	D3DLOCKED_RECT lockedRect;
	res = offscreen->LockRect(&lockedRect, nullptr, 0);
	uint32_t px = *(reinterpret_cast<uint32_t*>(lockedRect.pBits) + 32);
	res = offscreen->UnlockRect();*/


	dxvk::Com<IDirect3DSurface9> intermediate;
	if (false) {
		desc.Width = presentParams.BackBufferHeight;
		desc.Height = presentParams.BackBufferHeight;
	}
	else if (false) {
		desc.MultiSampleType = D3DMULTISAMPLE_NONE;
	}
	else {
		desc.Width = 0;
	}
	if (desc.Width != 0) {
		res = this->device->CreateRenderTarget(desc.Width, desc.Height, desc.Format, desc.MultiSampleType, desc.MultiSampleQuality, false, &intermediate, nullptr);
		res = this->device->StretchRect(rt.ptr(), nullptr, intermediate.ptr(), nullptr, D3DTEXF_LINEAR);
	}
	else {
		intermediate = rt;
	}

	res = this->device->SetRenderTarget(0, backbuffer.ptr());
	res = this->device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00ff00ff, 1.0f, 0);
	RECT resolveTarget = {
		0, 0, presentParams.BackBufferHeight, presentParams.BackBufferHeight
	};
	res = this->device->StretchRect(intermediate.ptr(), nullptr, backbuffer.ptr(), &resolveTarget, D3DTEXF_POINT);

	this->device->Present(nullptr, nullptr, nullptr, nullptr);
}
