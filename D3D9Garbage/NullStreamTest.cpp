#include "NullStreamTest.h"

#include <array>

#include <fstream>
#include <vector>

NullStreamTest::NullStreamTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device)) {

	HRESULT res;

	res = this->device->CreateVertexBuffer(16 * 6, 0, 0, D3DPOOL_MANAGED, &this->vb, nullptr);
	res = this->device->CreateVertexBuffer(16 * 6, 0, 0, D3DPOOL_MANAGED, &this->vb2, nullptr);
	res = this->device->CreateVertexBuffer(16 * 6, 0, 0, D3DPOOL_MANAGED, &this->colorvb, nullptr);

	void* ptr = nullptr;
	res = this->vb->Lock(0, 0, &ptr, 0);

	std::array<std::array<float, 4>, 6> data = {
		-1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 1.0f
	};

	memcpy(ptr, data.data(), 16 * 6);

	this->vb->Unlock();
	res = this->vb2->Lock(0, 0, &ptr, 0);
	data[0] = { 0.0f, 0.0f, 0.0f, 1.0f };
	memcpy(ptr, data.data(), 16 * 6);
	this->vb2->Unlock();


	res = this->colorvb->Lock(0, 0, &ptr, 0);
	data = {
		0.0f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f,
		0.5f, 1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f
	};
	memcpy(ptr, data.data(), 16 * 6);
	this->colorvb->Unlock();

	this->device->CreateIndexBuffer(6 * 4, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &this->ib, nullptr);
	this->device->CreateIndexBuffer(6 * 4, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &this->ib2, nullptr);
	std::array<uint32_t, 6> ib_data = {
		0, 1, 2, 3, 4, 5
	};
	this->ib->Lock(0, 0, &ptr, 0);
	memcpy(ptr, ib_data.data(), 6 * 4);
	this->ib->Unlock();
	this->ib2->Lock(0, 0, &ptr, 0);
	ib_data[0] = 3;
	memcpy(ptr, ib_data.data(), 6 * 4);
	this->ib2->Unlock();

	std::array<D3DVERTEXELEMENT9, 3> elements = {
		0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0,
		1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0,
		D3DDECL_END()
	};
	res = this->device->CreateVertexDeclaration(elements.data(), &this->vertexDecl);

	auto vsData = readFile("nullstream_vs.dxso");
	res = this->device->CreateVertexShader(reinterpret_cast<DWORD*>(vsData.data()), &this->vs);
	auto psData = readFile("nullstream_ps.dxso");
	res = this->device->CreatePixelShader(reinterpret_cast<DWORD*>(psData.data()), &this->ps);
	auto vs2Data = readFile("atiatoc_vs.dxso");
	res = this->device->CreateVertexShader(reinterpret_cast<DWORD*>(vs2Data.data()), &this->singleStreamVS);
	auto ps2Data = readFile("atiatoc_ps.dxso");
	res = this->device->CreatePixelShader(reinterpret_cast<DWORD*>(ps2Data.data()), &this->singleStreamPS);

	res = this->device->CreateRenderTarget(1280, 720, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &this->rt, nullptr);
}

void NullStreamTest::Render() {
	HRESULT res;

	res = device->TestCooperativeLevel();

	dxvk::Com<IDirect3DSurface9> backbuffer;
	res = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	D3DSURFACE_DESC surfDesc;
	res = backbuffer->GetDesc(&surfDesc);

	res = device->BeginScene();
	res = device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	res = device->SetRenderTarget(0, backbuffer.ptr());
	D3DRECT rect = {
		0, 0, 1280, 720
	};
	device->Clear(1, &rect, D3DCLEAR_TARGET, 0x00FF00, 0.0f, 0);
	device->SetRenderTarget(0, backbuffer.ptr());
	res = device->SetVertexDeclaration(vertexDecl.ptr());
	res = device->SetVertexShader(vs.ptr());
	res = device->SetPixelShader(ps.ptr());
	res = device->SetStreamSource(0, vb.ptr(), 0, 16);
	res = device->SetStreamSource(1, colorvb.ptr(), 0, 16);
	res = device->SetIndices(this->ib.ptr());

	std::array<float, 4> constData = { 0.0f, 1.0f, 1.0f, 1.0f };
	device->SetPixelShaderConstantF(0, constData.data(), 1);
	res = device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);

	device->SetStreamSource(0, nullptr, 0, 16);
	device->SetStreamSource(1, nullptr, 0, 16);
	//device->SetIndices(nullptr);
	res = device->SetVertexShader(vs.ptr());
	res = device->SetPixelShader(ps.ptr());


	
	//res = device->EndScene();
	//device->Present(nullptr, nullptr, nullptr, nullptr);
	//res = device->BeginScene();

	//constData = { 1.0f, 0.0f, 1.0f, 1.0f };
	//res = device->SetPixelShaderConstantF(0, constData.data(), 1);
	//res = device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);

	//res = device->EndScene();
	backbuffer = nullptr;
	rt = nullptr;

	D3DPRESENT_PARAMETERS params = {
	1280, 720, D3DFMT_UNKNOWN, 1, D3DMULTISAMPLE_NONE, 0, D3DSWAPEFFECT_DISCARD, window, true, false, D3DFMT_UNKNOWN, 0, 0, 1
	};
	//res = device->Reset(&params);

	//res = device->EndScene();
	//res = device->BeginScene();

	dxvk::Com<IDirect3DVertexBuffer9> getVB;
	UINT offset, stride;
	res = device->GetStreamSource(0, &getVB, &offset, &stride);

	constData = { 1.0f, 0.0f, 1.0f, 1.0f };
	res = device->SetPixelShaderConstantF(0, constData.data(), 1);
	res = device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);

	//res = device->EndScene();
	res = device->EndScene();

	device->SetRenderTarget(0, rt.ptr());
	res = device->Present(nullptr, nullptr, nullptr, nullptr);

	/*dxvk::Com<IDirect3DSurface9> backbuffer;

	device->BeginScene();

	device->SetRenderTarget(0, rt.ptr());
	D3DRECT rect = {
		0, 0, 1280, 720
	};
	device->Clear(1, &rect, D3DCLEAR_TARGET, 0x00FF00, 0.0f, 0);

	std::array<float, 4> constData = { 1.0f, 0.0f, 0.0f, 1.0f };
	device->SetPixelShaderConstantF(0, constData.data(), 1);

	device->EndScene();
	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	device->StretchRect(rt.ptr(), nullptr, backbuffer.ptr(), nullptr, D3DTEXF_LINEAR);
	device->Present(nullptr, nullptr, nullptr, nullptr);
	device->BeginScene();
	device->SetRenderTarget(0, rt.ptr());

	HRESULT res;

	res = device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	//res = device->SetVertexShader(singleStreamVS.ptr());
	//res = device->SetPixelShader(singleStreamPS.ptr());
	res = device->SetVertexShader(vs.ptr());
	res = device->SetPixelShader(ps.ptr());
	res = device->SetVertexDeclaration(vertexDecl.ptr());
	res = device->SetStreamSource(0, vb.ptr(), 0, 16);
	res = device->SetStreamSource(1, colorvb.ptr(), 0, 16);
	res = device->SetIndices(this->ib.ptr());
	res = device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);


	//device->EndScene();
	//device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	//device->StretchRect(rt.ptr(), nullptr, backbuffer.ptr(), nullptr, D3DTEXF_LINEAR);
	//device->Present(nullptr, nullptr, nullptr, nullptr);
	//device->Clear(1, &rect, D3DCLEAR_TARGET, 0x00FF00, 0.0f, 0);
	//device->BeginScene();
	//device->SetRenderTarget(0, rt.ptr());


	uint64_t refs = this->vb->AddRef() - 1;
	this->vb->Release();

	constData = { 0.0f, 0.0f, 1.0f, 1.0f };
	res = device->SetPixelShaderConstantF(0, constData.data(), 1);
	res = device->SetStreamSource(0, nullptr, 0, 0);
	res = device->SetStreamSource(1, nullptr, 0, 0);
	res = device->SetIndices(nullptr);
	//res = device->SetStreamSource(0, nullptr, 16, 16);
	res = device->SetVertexShader(vs.ptr());
	res = device->SetPixelShader(ps.ptr());

	dxvk::Com<IDirect3DVertexBuffer9> currentlyBoundVB;
	UINT stride;
	UINT offset;
	res = device->GetStreamSource(0, &currentlyBoundVB, &offset, &stride);

	dxvk::Com<IDirect3DIndexBuffer9> currentlyBoundIB;
	res = device->GetIndices(&currentlyBoundIB);

	//device->SetIndices(this->ib2.ptr());
	//device->SetStreamSource(0, this->vb2.ptr(), 0, 16);

	uint64_t refs2 = this->vb->AddRef() - 1;
	this->vb->Release();

	res = device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);
	res = device->SetStreamSource(1, nullptr, 0, 0);


	device->EndScene();
	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	device->StretchRect(rt.ptr(), nullptr, backbuffer.ptr(), nullptr, D3DTEXF_LINEAR);
	device->Present(nullptr, nullptr, nullptr, nullptr);
	device->BeginScene();
	device->SetRenderTarget(0, rt.ptr());

	//res = device->SetStreamSource(0, nullptr, 16, 16);

	constData = { 1.0f, 0.0f, 1.0f, 1.0f };
	res = device->SetPixelShaderConstantF(0, constData.data(), 1);
	res = device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);


	device->EndScene();
	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	device->StretchRect(rt.ptr(), nullptr, backbuffer.ptr(), nullptr, D3DTEXF_LINEAR);
	device->Present(nullptr, nullptr, nullptr, nullptr);
	device->BeginScene();
	device->SetRenderTarget(0, rt.ptr());


	if (this->frame == 0 && false) {
		D3DPRESENT_PARAMETERS params = {
		1280, 720, D3DFMT_UNKNOWN, 1, D3DMULTISAMPLE_NONE, 0, D3DSWAPEFFECT_DISCARD, window, true, false, D3DFMT_UNKNOWN, 0, 0, 1
		};
		HRESULT resetRes = device->Reset(&params);
	}

	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	device->StretchRect(rt.ptr(), nullptr, backbuffer.ptr(), nullptr, D3DTEXF_LINEAR);

	device->EndScene();
	device->Present(nullptr, nullptr, nullptr, nullptr);
	frame++;*/
}
