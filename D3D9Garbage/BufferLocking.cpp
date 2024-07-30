#include "BufferLocking.h"

#include <stdint.h>
#include <array>
#include <vector>

struct Vec4 {
	float x;
	float y;
	float z;
	float w;
};

struct Vertex4 {
	Vec4 position;
	Vec4 texcoord;
};


BufferLocking::BufferLocking(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device))
{
	HRESULT res;

	std::array<Vertex4, 3> quad = {
		-1.0f, -1.0f, 0.0f, 1.0f,    0.2f, 0.4f, 0.6f, 0.8f,
		-1.0f,  1.0f, 0.0f, 1.0f,    0.2f, 0.4f, 0.6f, 0.8f,
		 1.0f,  1.0f, 1.0f, 1.0f,    0.2f, 0.4f, 0.6f, 0.8f,
	};
	std::array<uint16_t, 3> indices = { 0, 1, 2 };

	res = this->device->CreateVertexBuffer(sizeof(quad), 0, 0, D3DPOOL_DEFAULT, &this->vb, nullptr);
	void* vbPtr = nullptr;
	this->vb->Lock(0, 0, &vbPtr, sizeof(quad));
	memcpy(vbPtr, quad.data(), sizeof(quad));
	this->vb->Unlock();

	void* ibPtr = nullptr;
	res = this->device->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &this->ib, nullptr);
	this->ib->Lock(0, 0, &ibPtr, 0);
	memcpy(ibPtr, indices.data(), sizeof(indices));
	this->ib->Unlock();


	std::array<D3DVERTEXELEMENT9, 3> elements = {
		0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0,
		0, sizeof(Vec4), D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0,
		D3DDECL_END()
	};
	res = this->device->CreateVertexDeclaration(elements.data(), &this->vertexDecl);

	auto vsData = readFile("nullstream_vs.dxso");
	auto psData = readFile("nullstream_ps.dxso");
	res = this->device->CreateVertexShader(reinterpret_cast<DWORD*>(vsData.data()), &this->vs);
	res = this->device->CreatePixelShader(reinterpret_cast<DWORD*>(psData.data()), &this->ps);
}

void BufferLocking::Render()
{
	std::array<Vertex4, 3> quad = {
		-1.0f, -1.0f, 0.0f, 1.0f,    0.6f, 0.6f, 0.6f, 0.8f,
		-1.0f,  1.0f, 0.0f, 1.0f,    0.6f, 0.6f, 0.6f, 0.8f,
		 1.0f,  1.0f, 1.0f, 1.0f,    0.6f, 0.6f, 0.6f, 0.8f,
	};
	std::array<uint16_t, 3> indices = { 0, 1, 2 };

	HRESULT res;
	res = this->device->SetVertexShader(vs.ptr());
	res = this->device->SetPixelShader(ps.ptr());
	res = this->device->SetVertexDeclaration(vertexDecl.ptr());
	res = this->device->SetStreamSource(0, vb.ptr(), 0, sizeof(Vertex4));
	//this->device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | 1);
	res = this->device->SetIndices(ib.ptr());

	dxvk::Com<IDirect3DSurface9> backbuffer;
	res = this->device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	res = this->device->SetRenderTarget(0, backbuffer.ptr());
	res = this->device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	RECT scissor = { 0, 0, 640, 480 };
	res = this->device->SetScissorRect(&scissor);
	D3DVIEWPORT9 viewport = { 0, 0, 640, 480, 0.0f, 1.0f };
	res = this->device->SetViewport(&viewport);

	float val[] = {1.0f, 1.0f, 1.0f, 1.0f};
	res = this->device->SetPixelShaderConstantF(0, &val[0], 1);

	res = this->device->BeginScene();
	D3DRECT rect = {
		0, 0, 640, 480
	};
	res = this->device->Clear(1, &rect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FF00, 1.0f, 0);
	res = this->device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 1);
	res = this->device->EndScene();

	void* vbPtr = nullptr;
	this->vb->Lock(0, 0, &vbPtr, D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE);
	memcpy(vbPtr, quad.data(), sizeof(quad));
	this->vb->Unlock();

	void* ibPtr = nullptr;
	this->ib->Lock(0, 0, &ibPtr, D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE);
	memcpy(ibPtr, indices.data(), 9);
	this->ib->Unlock();

	res = device->Present(nullptr, nullptr, nullptr, nullptr);

	frame++;
}

