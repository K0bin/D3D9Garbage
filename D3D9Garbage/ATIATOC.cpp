#include "ATIATOC.h"

#include <array>

#include <fstream>
#include <vector>

enum class ATOCMode {
	None,
	NV,
	AMD
};

enum class ATOCTestFlag : uint32_t {
	None = 0,
	AlphaTest = 1,
	MSAA = 2,
	Blending = 4
};

const ATOCMode Mode = ATOCMode::AMD;
const uint32_t TestFlags = ((uint32_t)ATOCTestFlag::MSAA);

ATIATOC::ATIATOC(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device)) {

	HRESULT res;

	res = this->device->CreateVertexBuffer(16 * 6, 0, 0, D3DPOOL_MANAGED, &this->vb, nullptr);

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

	std::array<D3DVERTEXELEMENT9, 2> elements = {
		0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0,
		D3DDECL_END()
	};
	res = this->device->CreateVertexDeclaration(elements.data(), &this->vertexDecl);

	auto vsData = readFile("atiatoc_vs.dxso");
	res = this->device->CreateVertexShader(reinterpret_cast<DWORD*>(vsData.data()), &this->vs);
	auto psData = readFile("atiatoc_ps.dxso");
	res = this->device->CreatePixelShader(reinterpret_cast<DWORD*>(psData.data()), &this->ps);

	res = this->device->CreateRenderTarget(1280, 720, D3DFMT_A8R8G8B8, ((TestFlags & ((uint32_t)ATOCTestFlag::MSAA)) != 0) ? D3DMULTISAMPLE_4_SAMPLES : D3DMULTISAMPLE_NONE, 0, false, &this->rt, nullptr);
}

void ATIATOC::Render() {
	device->BeginScene();

	device->SetRenderTarget(0, rt.ptr());
	D3DRECT rect = {
		0, 0, 1280, 720
	};
	device->Clear(1, &rect, D3DCLEAR_TARGET, 0x00FF00, 0.0f, 0);

	if ((TestFlags & ((uint32_t) ATOCTestFlag::AlphaTest)) != 0) {
		device->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
	}

	if (Mode == ATOCMode::NV) {
		device->SetRenderState(D3DRS_ADAPTIVETESS_Y, MAKEFOURCC('A', 'T', 'O', 'C'));
	}
	else if (Mode == ATOCMode::AMD) {
		device->SetRenderState(D3DRS_POINTSIZE, MAKEFOURCC('A', '2', 'M', '1'));
	}

	if ((TestFlags & ((uint32_t)ATOCTestFlag::Blending)) != 0) {
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}

	std::array<float, 4> constData = { 0.25f, 0.0f, 0.0f, 0.0f };
	device->SetPixelShaderConstantF(0, constData.data(), 1);

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	device->SetVertexShader(vs.ptr());
	device->SetPixelShader(ps.ptr());
	device->SetStreamSource(0, vb.ptr(), 0, 16);
	device->SetVertexDeclaration(vertexDecl.ptr());
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	dxvk::Com<IDirect3DSurface9> backbuffer;
	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	device->StretchRect(rt.ptr(), nullptr, backbuffer.ptr(), nullptr, D3DTEXF_LINEAR);

	device->EndScene();
	device->Present(nullptr, nullptr, nullptr, nullptr);
}
