#include "FFTest.h"

#include <array>

#include <fstream>
#include <vector>

FFTest::FFTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device)) {
}

void FFTest::Render() {
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

	void* ibPtr = nullptr;
	res = this->device->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &this->ib, nullptr);
	this->ib->Lock(0, 0, &ibPtr, 0);
	memcpy(ibPtr, indices.data(), sizeof(indices));

	res = this->device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	res = this->device->SetRenderState(D3DRS_LIGHTING, false);
	res = this->device->SetStreamSource(0, this->vb.ptr(), 0, sizeof(Vertex));
	res = this->device->SetIndices(this->ib.ptr());
	res = this->device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff0000ff, 1.0f, 0);
	res = this->device->BeginScene();
	res = this->device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 1);
	res = this->device->EndScene();

	dxvk::Com<IDirect3DSurface9> rt;
	res = this->device->GetRenderTarget(0, &rt);
	D3DSURFACE_DESC desc;
	res = rt->GetDesc(&desc);
	dxvk::Com<IDirect3DSurface9> rb;
	res = this->device->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &rb, nullptr);
	res = this->device->GetRenderTargetData(rt.ptr(), rb.ptr());
	D3DLOCKED_RECT locked;
	res = rb->LockRect(&locked, nullptr, D3DLOCK_READONLY);
	uint32_t x = 160;
	uint32_t y = 120;
	DWORD color = ((DWORD*)locked.pBits)[y * locked.Pitch / sizeof(DWORD) + x] & 0x00ffffff;
	res = rb->UnlockRect();

	this->device->Present(nullptr, nullptr, nullptr, nullptr);
	this->vb->Unlock();
	this->ib->Unlock();
}
