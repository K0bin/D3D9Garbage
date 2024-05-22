#include "FFTexcoordTest.h"

#include <stdint.h>
#include <array>

#include <fstream>
#include <vector>

FFTexcoordTest::FFTexcoordTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device)) {
}

void FFTexcoordTest::Render() {
	HRESULT res;

	std::array<Vertex4, 3> quad = {
		-1.0f, -1.0f, 0.1f, 1.0f,    0.0f, 0.0f, 0.0f, 2.0f,
		-1.0f,  1.0f, 0.1f, 1.0f,    0.0f, 1.0f, 0.0f, 2.0f,
		 1.0f,  1.0f, 0.1f, 1.0f,    1.0f, 1.0f, 0.0f, 2.0f,
	};
	std::array<uint16_t, 3> indices = { 0, 1, 2 };

	//res = this->device->CreateVertexBuffer(sizeof(quad), D3DUSAGE_DYNAMIC, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &this->vb, nullptr);
	//res = this->device->CreateVertexBuffer(sizeof(quad), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &this->vb, nullptr);
	//res = this->device->CreateVertexBuffer(sizeof(quad), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &this->vb, nullptr);
	res = this->device->CreateVertexBuffer(sizeof(quad), 0, D3DFVF_XYZW | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE4(0), D3DPOOL_SYSTEMMEM, &this->vb, nullptr);
	void* vbPtr = nullptr;
	this->vb->Lock(0, 0, &vbPtr, sizeof(quad));
	memcpy(vbPtr, quad.data(), sizeof(quad));
	this->vb->Unlock();

	void* ibPtr = nullptr;
	res = this->device->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &this->ib, nullptr);
	this->ib->Lock(0, 0, &ibPtr, 0);
	memcpy(ibPtr, indices.data(), sizeof(indices));
	this->ib->Unlock();


	res = this->device->CreateTexture(16, 16, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &this->texture, nullptr);
	D3DLOCKED_RECT lockedRect = { 0 };
	res = this->texture->LockRect(0, &lockedRect, nullptr, 0);
	for (uint32_t y = 0; y < 16; y++) {
		for (uint32_t x = 0; x < 16; x++) {
			uint8_t* ptr = reinterpret_cast<uint8_t*>(lockedRect.pBits);
			ptr += y * lockedRect.Pitch + x * 4;

			ptr[0] = uint8_t((float(x) / 15.0f) * 255.0f);
			ptr[1] = uint8_t((float(y) / 15.0f) * 255.0f);
			ptr[2] = 0;
			ptr[3] = 255;
		}
	}
	this->texture->UnlockRect(0);
	this->device->SetTexture(0, this->texture.ptr());

	D3DMATERIAL9 material = {
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f
	};
	this->device->SetMaterial(&material);
	res = this->device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	res = this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	//res = this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
	//res = this->device->SetTextureStageState(0, D3DTSS_COLOROP, COLOROPT);
	//res = this->device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT4);
	//res = this->device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3);
	//res = this->device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT1);
	//res = this->device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT1 | D3DTTFF_PROJECTED);
	//res = this->device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	//res = this->device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 | D3DTTFF_PROJECTED);
	//res = this->device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3 | D3DTTFF_PROJECTED);
	//res = this->device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT4 | D3DTTFF_PROJECTED);
	//res = this->device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, 0xff | D3DTTFF_PROJECTED);
	//res = this->device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, 0xff);
	res = this->device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED);

	D3DMATRIX mat = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
	};
	//res = this->device->SetTransform(D3DTS_TEXTURE0, &mat);

	res = this->device->SetRenderState(D3DRS_LIGHTING, false);
	res = this->device->SetStreamSource(0, this->vb.ptr(), 0, sizeof(Vertex4));
	res = this->device->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE4(0));
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
}
