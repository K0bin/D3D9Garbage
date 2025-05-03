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
	MSAAQuality = 4,
	Blending = 8,
};

const ATOCMode Mode = ATOCMode::NV;
const uint32_t TestFlags = ((uint32_t)ATOCTestFlag::AlphaTest) | ((uint32_t)ATOCTestFlag::MSAAQuality) | ((uint32_t)ATOCTestFlag::None);

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

	D3DMULTISAMPLE_TYPE msaaMode = D3DMULTISAMPLE_NONE;
	if ((TestFlags & ((uint32_t)ATOCTestFlag::MSAA)) != 0)
		msaaMode = D3DMULTISAMPLE_4_SAMPLES;
	else if ((TestFlags & ((uint32_t)ATOCTestFlag::MSAAQuality)) != 0)
		msaaMode = D3DMULTISAMPLE_NONMASKABLE;

	DWORD msaaQuality = 0;
	if ((TestFlags & ((uint32_t)ATOCTestFlag::MSAAQuality)) != 0)
		msaaQuality = 2;

	res = this->device->CreateRenderTarget(1280, 720, D3DFMT_X8R8G8B8, msaaMode, msaaQuality, false, &this->rt, nullptr);
}

void ATIATOC::Render() {
	HRESULT hr;

	hr = device->BeginScene();

	hr = device->SetDepthStencilSurface(nullptr);
	hr = device->SetRenderTarget(0, rt.ptr());
	D3DRECT rect = {
		0, 0, 1280, 720
	};
	hr = device->Clear(1, &rect, D3DCLEAR_TARGET, 0x00FF00, 0.0f, 0);

	D3DVIEWPORT9 viewport = {
		0, 0, 1280, 720, 0.0f, 1.0f
	};
	hr = device->SetViewport(&viewport);

	RECT scissorRect = {
		0, 0, 1280, 720
	};
	hr = device->SetScissorRect(&scissorRect);

	if ((TestFlags & ((uint32_t) ATOCTestFlag::AlphaTest)) != 0) {
		hr = device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	}

	if (Mode == ATOCMode::NV) {
		hr = device->SetRenderState(D3DRS_ADAPTIVETESS_Y, MAKEFOURCC('A', 'T', 'O', 'C'));
	}
	else if (Mode == ATOCMode::AMD) {
		hr = device->SetRenderState(D3DRS_POINTSIZE, MAKEFOURCC('A', '2', 'M', '1'));
	}

	if ((TestFlags & ((uint32_t)ATOCTestFlag::Blending)) != 0) {
		hr = device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		hr = device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		hr = device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		hr = device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}

	std::array<float, 4> constData = { 0.5f, 0.0f, 0.0f, 0.0f };
	hr = device->SetPixelShaderConstantF(0, constData.data(), 1);

	hr = device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	hr = device->SetVertexShader(vs.ptr());
	hr = device->SetPixelShader(ps.ptr());
	hr = device->SetStreamSource(0, vb.ptr(), 0, 16);
	hr = device->SetVertexDeclaration(vertexDecl.ptr());
	hr = device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	hr = device->EndScene();

	dxvk::Com<IDirect3DSurface9> backbuffer;
	hr = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	hr = device->StretchRect(rt.ptr(), nullptr, backbuffer.ptr(), nullptr, D3DTEXF_LINEAR);

	hr = device->Present(nullptr, nullptr, nullptr, nullptr);
}
