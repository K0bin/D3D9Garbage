#include "StateBlockVertexOffsetTest.h"

#include <array>
#include <iostream>

StateBlockVertexOffsetTest::StateBlockVertexOffsetTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device)) {
	HRESULT res = this->device->CreateVertexBuffer(256, 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &this->vb, nullptr);
	res = this->device->CreateVertexBuffer(512, 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &this->vb2, nullptr);

	D3DCAPS9 caps;
	res = this->device->GetDeviceCaps(&caps);

	DWORD val;
	res = this->device->GetRenderState(D3DRS_POINTSIZE_MAX, &val);

	DWORD test = 0x43800000;
	DWORD test2 = 0x42800000;
	float testFloat = *((float*)&test);
	float testFloat2 = *((float*)&test2);

	std::array<D3DVERTEXELEMENT9, 3> elements;	
	elements[0] = { 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION };
	elements[1] = { 0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL };
	elements[2] = D3DDECL_END();

	res = this->device->CreateVertexDeclaration(elements.data(), &this->vd);

	std::array<D3DVERTEXELEMENT9, 4> elements1;
	elements1[0] = { 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION };
	elements1[1] = { 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR};
	elements1[2] = { 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD};
	elements1[3] = D3DDECL_END();
	res = this->device->CreateVertexDeclaration(elements1.data(), &this->vd2);

	std::array<D3DVERTEXELEMENT9, 4> elements2;
	elements2[0] = { 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION };
	elements2[1] = { 1, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR };
	elements2[2] = { 2, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD };
	elements2[3] = D3DDECL_END();
	res = this->device->CreateVertexDeclaration(elements2.data(), &this->vd3);
}

void StateBlockVertexOffsetTest::Render() {
	HRESULT res = this->device->SetStreamSource(0, this->vb.ptr(), 64, 32);
	res = this->device->SetVertexDeclaration(this->vd.ptr());
	//res = this->device->CreateStateBlock(D3DSBT_ALL, &this->sb);

	HRESULT setRes = this->device->SetTextureStageState(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS, 0b111);
	DWORD test;
	HRESULT getRes = this->device->GetTextureStageState(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS, &test);

	res = this->device->BeginStateBlock();
	//res = this->device->SetStreamSource(0, this->vb.ptr(), 8, 8);
	//res = this->device->SetVertexDeclaration(this->vd2.ptr());
	res = this->device->EndStateBlock(&this->sb);

	std::array<float, 4 * 8> data = {
		-1.0f, -1.0f, 0.1f, 1.0f,    0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.1f, 1.0f,    1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.1f, 1.0f,    1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.1f, 1.0f,    0.0f, 1.0f, 0.0f, 0.0f,
	};
	//this->device->DrawIndexedPrimitiveUP()


	res = this->device->SetStreamSource(0, this->vb2.ptr(), 128, 48);
	res = this->device->SetVertexDeclaration(this->vd3.ptr());
	res = this->sb->Capture();
	res = this->device->SetStreamSource(0, this->vb.ptr(), 0, 16);
	//res = this->device->SetVertexDeclaration(this->vd.ptr());
	res = this->sb->Apply();

	dxvk::Com<IDirect3DVertexBuffer9> testVB;
	UINT testOffset;
	UINT testStride;
	dxvk::Com<IDirect3DVertexDeclaration9> testVD;
	res = this->device->GetStreamSource(0, &testVB, &testOffset, &testStride);
	res = this->device->GetVertexDeclaration(&testVD);

	std::cout << "Offset: " << testOffset << "\n";
}
