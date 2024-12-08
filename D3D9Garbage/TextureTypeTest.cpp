#include "TextureTypeTest.h"

#include <array>
#include <iostream>
#include <filesystem>

enum class TextureDimension {
	Dim2D,
	Dim3D,
	DimCube
};

static TextureDimension TextureToBind = TextureDimension::Dim2D;
static TextureDimension TextureInShader = TextureDimension::DimCube;
static uint32_t ShaderModel = 3;

TextureTypeTest::TextureTypeTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device)) {
	HRESULT res;

	std::array<TextureTypeVertex, 4> quad = {
		-1.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f, 0.0f,
		1.0f,  -1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 0.0f,
		-1.0f,  -1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 0.0f
	};
	std::array<uint16_t, 6> indices = { 0, 1, 2, 2, 3, 0 };

	res = this->device->CreateVertexBuffer(sizeof(quad), 0, 0, D3DPOOL_SYSTEMMEM, &this->vb, nullptr);
	void* vbPtr = nullptr;
	this->vb->Lock(0, 0, &vbPtr, sizeof(quad));
	memcpy(vbPtr, quad.data(), sizeof(quad));
	this->vb->Unlock();

	void* ibPtr = nullptr;
	res = this->device->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &this->ib, nullptr);
	this->ib->Lock(0, 0, &ibPtr, 0);
	memcpy(ibPtr, indices.data(), sizeof(indices));
	this->ib->Unlock();

	auto current_dir = std::filesystem::current_path();
	auto vsData = readFile("texture_type_vs.dxso");
	res = this->device->CreateVertexShader(reinterpret_cast<DWORD*>(vsData.data()), &this->vs);
	vsData = readFile("texture_type_vs_3.dxso");
	res = this->device->CreateVertexShader(reinterpret_cast<DWORD*>(vsData.data()), &this->vs_3);
	auto psData = readFile("texture_type_ps.dxso");
	res = this->device->CreatePixelShader(reinterpret_cast<DWORD*>(psData.data()), &this->ps);
	psData = readFile("texture_type_ps_3.dxso");
	res = this->device->CreatePixelShader(reinterpret_cast<DWORD*>(psData.data()), &this->ps_3);
	psData = readFile("texture_type_ps_3d.dxso");
	res = this->device->CreatePixelShader(reinterpret_cast<DWORD*>(psData.data()), &this->ps_3d);
	psData = readFile("texture_type_ps_3d_3.dxso");
	res = this->device->CreatePixelShader(reinterpret_cast<DWORD*>(psData.data()), &this->ps_3d_3);
	psData = readFile("texture_type_ps_cube.dxso");
	res = this->device->CreatePixelShader(reinterpret_cast<DWORD*>(psData.data()), &this->ps_cube);
	psData = readFile("texture_type_ps_cube_3.dxso");
	res = this->device->CreatePixelShader(reinterpret_cast<DWORD*>(psData.data()), &this->ps_cube_3);

	res = this->device->CreateTexture(16, 16, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &this->texture2d, nullptr);
	D3DLOCKED_RECT lockedRect;
	res = this->texture2d->LockRect(0, &lockedRect, nullptr, 0);
	for (uint32_t y = 0; y < 16; y++) {
		for (uint32_t x = 0; x < 16; x++) {
			uint8_t* ptr = reinterpret_cast<uint8_t*>(lockedRect.pBits);
			ptr += lockedRect.Pitch * y;
			ptr += x * 4;

			ptr[0] = 0xFF;
			ptr[1] = 0xFF;
			ptr[2] = 0xAA;
			ptr[3] = 0xFF;
		}
	}
	res = this->texture2d->UnlockRect(0);

	res = this->device->CreateVolumeTexture(16, 16, 16, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &this->texture3d, nullptr);
	D3DLOCKED_BOX lockedBox;
	res = this->texture3d->LockBox(0, &lockedBox, nullptr, 0);
	for (uint32_t z = 0; z < 16; z++) {
		for (uint32_t y = 0; y < 16; y++) {
			for (uint32_t x = 0; x < 16; x++) {
				uint8_t* ptr = reinterpret_cast<uint8_t*>(lockedBox.pBits);
				ptr += lockedBox.SlicePitch * z;
				ptr += lockedBox.RowPitch * y;
				ptr += x * 4;

				ptr[0] = 0xFF;
				ptr[1] = (z+1) * 16;
				ptr[2] = 0xFF;
				ptr[3] = 0xFF;
			}
		}
	}
	res = this->texture3d->UnlockBox(0);

	res = this->device->CreateCubeTexture(16, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &this->textureCube, nullptr);
	for (uint32_t face = 0; face < 6; face++) {
		lockedRect = {};
		res = this->textureCube->LockRect((D3DCUBEMAP_FACES) face, 0, &lockedRect, nullptr, 0);
		for (uint32_t y = 0; y < 16; y++) {
			for (uint32_t x = 0; x < 16; x++) {
				uint8_t* ptr = reinterpret_cast<uint8_t*>(lockedRect.pBits);
				ptr += lockedRect.Pitch * y;
				ptr += x * 4;

				ptr[0] = (face+1) * 42;
				ptr[1] = 0xFF;
				ptr[2] = 0xAA;
				ptr[3] = 0xFF;
			}
		}
		res = this->textureCube->UnlockRect((D3DCUBEMAP_FACES) face, 0);
	}

	std::array<D3DVERTEXELEMENT9, 3> vertexElements = {
		0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0,
		0, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0,
		D3DDECL_END()
	};
	res = this->device->CreateVertexDeclaration(vertexElements.data(), &this->vertexDecl);
}

void TextureTypeTest::Render() {
	HRESULT res;

	this->device->BeginScene();

	this->device->SetVertexDeclaration(this->vertexDecl.ptr());
	this->device->SetStreamSource(0, this->vb.ptr(), 0, sizeof(TextureTypeVertex));
	this->device->SetIndices(this->ib.ptr());

	if (ShaderModel == 3) {
		this->device->SetVertexShader(this->vs_3.ptr());
		switch (TextureInShader) {
		case TextureDimension::Dim2D:
			this->device->SetPixelShader(this->ps_3.ptr());
			break;

		case TextureDimension::Dim3D:
			this->device->SetPixelShader(this->ps_3d_3.ptr());
			break;

		case TextureDimension::DimCube:
			this->device->SetPixelShader(this->ps_cube_3.ptr());
			break;
		}
	}
	else {
		this->device->SetVertexShader(this->vs.ptr());
		switch (TextureInShader) {
		case TextureDimension::Dim2D:
			this->device->SetPixelShader(this->ps.ptr());
			break;

		case TextureDimension::Dim3D:
			this->device->SetPixelShader(this->ps_3d.ptr());
			break;

		case TextureDimension::DimCube:
			this->device->SetPixelShader(this->ps_cube.ptr());
			break;
		}
	}

	switch (TextureToBind) {
	case TextureDimension::Dim2D:
		this->device->SetTexture(0, this->texture2d.ptr());
		break;

	case TextureDimension::Dim3D:
		this->device->SetTexture(0, this->texture3d.ptr());
		break;

	case TextureDimension::DimCube:
		this->device->SetTexture(0, this->textureCube.ptr());
		break;
	}

	this->device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	this->device->EndScene();

	res = device->Present(nullptr, nullptr, nullptr, nullptr);
}
