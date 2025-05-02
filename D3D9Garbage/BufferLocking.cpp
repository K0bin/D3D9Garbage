#include "BufferLocking.h"

#include <stdint.h>
#include <array>
#include <vector>
#include <random>
#include <iostream>



struct Vec4 {
	float x;
	float y;
	float z;
	float w;
};

struct Vertex4 {
	Vec4 position;
	uint32_t color;
};

const uint32_t NUM_TRIS_TOTAL = 9;

BufferLocking::BufferLocking(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device))
{
	HRESULT res = S_OK;

	std::array<D3DVERTEXELEMENT9, 3> elements = {
		0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0,
		0, sizeof(Vec4), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0,
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
	TestBufferVariants(D3DPOOL_DEFAULT, 0, D3DPOOL_DEFAULT, 0);
	TestBufferVariants(D3DPOOL_DEFAULT, 0, D3DPOOL_DEFAULT, D3DUSAGE_DYNAMIC);
	TestBufferVariants(D3DPOOL_DEFAULT, 0, D3DPOOL_DEFAULT, D3DUSAGE_WRITEONLY);
	TestBufferVariants(D3DPOOL_DEFAULT, 0, D3DPOOL_DEFAULT, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC);
	TestBufferVariants(D3DPOOL_DEFAULT, 0, D3DPOOL_SYSTEMMEM, 0);
	TestBufferVariants(D3DPOOL_DEFAULT, 0, D3DPOOL_SYSTEMMEM, D3DUSAGE_DYNAMIC);
	TestBufferVariants(D3DPOOL_DEFAULT, 0, D3DPOOL_SYSTEMMEM, D3DUSAGE_WRITEONLY);
	TestBufferVariants(D3DPOOL_DEFAULT, 0, D3DPOOL_SYSTEMMEM, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC);
	TestBufferVariants(D3DPOOL_DEFAULT, 0, D3DPOOL_MANAGED, 0);
	TestBufferVariants(D3DPOOL_DEFAULT, 0, D3DPOOL_MANAGED, D3DUSAGE_DYNAMIC);
	TestBufferVariants(D3DPOOL_DEFAULT, 0, D3DPOOL_MANAGED, D3DUSAGE_WRITEONLY);
	TestBufferVariants(D3DPOOL_DEFAULT, 0, D3DPOOL_MANAGED, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC);

	std::cout << std::flush;
}

bool BufferLocking::TestBufferVariants(
	D3DPOOL iboPool,
	DWORD iboUsages,
	D3DPOOL vboPool,
	DWORD vboUsages
)
{
	TestResult res;
	uint32_t data;



	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::AfterUnlock,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Lock before draw, unlock immediately and write after unlock: " << uint32_t(res) << ", Data: " << data << "\n";
	return false;


	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::Beginning, UnlockMode::AfterLock, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::Beginning, UnlockMode::AfterLock, WriteMode::Never,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Sanity Check: " << uint32_t(res) << ", Data: " << data << "\n";

	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::Beginning, UnlockMode::AfterWrite, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::Beginning, UnlockMode::AfterWrite, WriteMode::AfterLock,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Regular: " << uint32_t(res) << ", Data: " << data << "\n";



	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::AfterLock,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Lock before draw, write, unlock immediately: " << uint32_t(res) << ", Data: " << data << "\n";
	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::AfterUnlock,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Lock before draw, unlock immediately and write after unlock: " << uint32_t(res) << ", Data: " << data << "\n";
	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::AfterDraw,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Lock before draw, unlock immediately and write after draw: " << uint32_t(res) << ", Data: " << data << "\n";
	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::Never,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Lock before draw, unlock immediately, never write: " << uint32_t(res) << ", Data: " << data << "\n";



	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::Beginning, UnlockMode::AfterWrite, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterDraw, WriteMode::AfterLock,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Draw while holding lock, write before draw: " << uint32_t(res) << ", Data: " << data << "\n";

	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::Beginning, UnlockMode::AfterDraw, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterDraw, WriteMode::AfterLock,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Draw while holding lock, write after draw: " << uint32_t(res) << ", Data: " << data << "\n";

	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::Beginning, UnlockMode::AfterLock, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::Beginning, UnlockMode::AfterLock, WriteMode::AfterUnlock,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Lock before scene, unlock immediately and write after unlock: " << uint32_t(res) << ", Data: " << data << "\n";

	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::AfterUnlock,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Lock before draw, unlock immediately and write after unlock: " << uint32_t(res) << ", Data: " << data << "\n";

	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::Beginning, UnlockMode::BeforeDraw, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::Beginning, UnlockMode::BeforeDraw, WriteMode::AfterUnlock,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Lock before scene, unlock before draw and write after unlock: " << uint32_t(res) << ", Data: " << data << "\n";

	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::Never,
		vboPool, vboUsages, 0, LockRangeMode::All, LockMode::BeforeDraw, UnlockMode::AfterLock, WriteMode::AfterDraw,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Lock before draw, unlock immediately and write after draw: " << uint32_t(res) << ", Data: " << data << "\n";

	data = D3DCOLOR_ARGB(16, 1, 2, 3);
	res = Test(
		iboPool, iboUsages, 0, LockRangeMode::All, LockMode::Beginning, UnlockMode::AfterDraw, WriteMode::Never,
		vboPool, vboUsages, D3DLOCK_DISCARD, LockRangeMode::All, LockMode::Beginning, UnlockMode::AfterLock, WriteMode::Never,
		DrawRangeMode::All, ReadResultMode::AfterDraw, &data
	);
	std::cout << "Test: Lock before scene with DISCARD, unlock immediately and never write anything: " << uint32_t(res) << ", Data: " << data << "\n";

	return true;
}

HRESULT BufferLocking::CreateSurface()
{
	HRESULT res = S_OK;

	dxvk::Com<IDirect3DSurface9> backbuffer;
	res = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	if (FAILED(res))
	{
		std::cerr << "Failed to get backbuffer" << res << "\n";
		return res;
	}

	D3DSURFACE_DESC desc;
	res = backbuffer->GetDesc(&desc);
	if (FAILED(res))
	{
		std::cerr << "Failed to get backbuffer desc" << res << "\n";
		return res;
	}

	res = device->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &surface, nullptr);
	if (FAILED(res))
	{
		std::cerr << "Failed to create surface" << res << "\n";
		return res;
	}
	return res;
}


HRESULT BufferLocking::CreateBuffers(
	D3DPOOL iboPool,
	DWORD iboUsages,
	D3DPOOL vboPool,
	DWORD vboUsages
)
{
	HRESULT res = S_OK;

	// 3 Triangles but we're only gonna draw one but want to experiment with indices
	res = device->CreateVertexBuffer(sizeof(Vertex4) * 3 * NUM_TRIS_TOTAL, vboUsages, 0, vboPool, &vb, nullptr);
	if (FAILED(res))
	{
		std::cerr << "Failed to create vertex buffer" << res << "\n";
		return res;
	}

	res = device->CreateIndexBuffer(sizeof(uint16_t) * 3 * NUM_TRIS_TOTAL, iboUsages, D3DFMT_INDEX16, iboPool, &ib, nullptr);
	if (FAILED(res))
	{
		std::cerr << "Failed to create index buffer" << res << "\n";
		return res;
	}
	return res;
}

HRESULT BufferLocking::PrefillBuffers()
{
	HRESULT res = S_OK;

	res = vb->Lock(0, 0, &vbPtr, 0);
	if (FAILED(res))
	{
		std::cerr << "Failed to lock vertex buffer for prefilling: " << res << "\n";
		return res;
	}
	std::array<Vertex4, 3> triangle = {
		-1.0f, -1.0f, 0.5f, 1.0f,    D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0),
		-1.0f,  1.0f, 0.5f, 1.0f,    D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0),
		 1.0f,  1.0f, 0.5f, 1.0f,    D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0),
	};
	memcpy(vbPtr, triangle.data(), sizeof(triangle));
	triangle = {
		-1.0f, -1.0f, 0.5f, 1.0f,    D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0xFF),
		-1.0f,  1.0f, 0.5f, 1.0f,    D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0xFF),
		 1.0f,  1.0f, 0.5f, 1.0f,    D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0xFF),
	};
	memcpy(reinterpret_cast<uint8_t*>(vbPtr) + sizeof(triangle), triangle.data(), sizeof(triangle));
	triangle = {
		-1.0f, -1.0f, 0.5f, 1.0f,    D3DCOLOR_ARGB(0xFF, 0xFF, 0xFF, 0),
		-1.0f,  1.0f, 0.5f, 1.0f,    D3DCOLOR_ARGB(0xFF, 0xFF, 0xFF, 0),
		 1.0f,  1.0f, 0.5f, 1.0f,    D3DCOLOR_ARGB(0xFF, 0xFF, 0xFF, 0),
	};
	memcpy(reinterpret_cast<uint8_t*>(vbPtr) + sizeof(triangle) * 2, triangle.data(), sizeof(triangle));
	res = vb->Unlock();
	if (FAILED(res))
	{
		std::cerr << "Failed to unlock vertex buffer for prefilling: " << res << "\n";
		return res;
	}


	res = ib->Lock(0, 0, &ibPtr, 0);
	if (FAILED(res))
	{
		std::cerr << "Failed to lock index buffer for prefilling: " << res << "\n";
		return res;
	}
	std::array<uint16_t, 3> indices = { 0, 1, 2 };
	memcpy(ibPtr, indices.data(), sizeof(indices));
	indices = { 3, 4, 5 };
	memcpy(reinterpret_cast<uint8_t*>(ibPtr) + sizeof(indices), indices.data(), sizeof(indices));
	indices = { 6, 7, 8 };
	memcpy(reinterpret_cast<uint8_t*>(ibPtr) + sizeof(indices) * 2, indices.data(), sizeof(indices));
	res = ib->Unlock();
	if (FAILED(res))
	{
		std::cerr << "Failed to unlock vertex buffer for prefilling: " << res << "\n";
		return res;
	}

	return res;
}

TestResult BufferLocking::Test(
	D3DPOOL iboPool,
	DWORD iboUsages,
	DWORD iboLockFlags,
	LockRangeMode iboLockRangeMode,
	LockMode iboLockMode,
	UnlockMode iboUnlockMode,
	WriteMode iboWriteMode,
	D3DPOOL vboPool,
	DWORD vboUsages,
	DWORD vboLockFlags,
	LockRangeMode vboLockRangeMode,
	LockMode vboLockMode,
	UnlockMode vboUnlockMode,
	WriteMode vboWriteMode,
	DrawRangeMode vbDawRangeMode,
	ReadResultMode readResultMode,
	uint32_t* result
)
{
	HRESULT res = S_OK;

	res = CreateBuffers(iboPool, iboUsages, vboPool, vboUsages);
	if (FAILED(res))
	{
		std::cerr << "Failed to create buffers" << res << "\n";
		return TestResult::CreationFailed;
	}

	res = PrefillBuffers();
	if (FAILED(res))
	{
		std::cerr << "Failed to prefill buffers" << res << "\n";
		return TestResult::LockFailed;
	}

	res = CreateSurface();
	if (FAILED(res))
	{
		std::cerr << "Failed to create surface" << res << "\n";
		return TestResult::Other;
	}

	void* vbData;
	void* ibData;

	// Do one frame before to ensure any per-scene/per-frame work for the prefill is done
	device->BeginScene();
	device->EndScene();
	device->Present(nullptr, nullptr, nullptr, nullptr);

	// Setup state
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
	res = this->device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	res = this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
	res = this->device->SetRenderState(D3DRS_ALPHATESTENABLE, 0);

	D3DSURFACE_DESC desc = {};
	res = backbuffer->GetDesc(&desc);

	RECT scissor = { 0, 0, desc.Width, desc.Height };
	res = this->device->SetScissorRect(&scissor);
	D3DVIEWPORT9 viewport = { 0, 0, desc.Width, desc.Height, 0.0f, 1.0f };
	res = this->device->SetViewport(&viewport);

	float val[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	res = this->device->SetPixelShaderConstantF(0, &val[0], 1);
	if (FAILED(res))
	{
		std::cerr << "Failed to setup draw state" << res << "\n";
		return TestResult::Other;
	}

	// Draw frame with initial resources
	res = device->BeginScene();
	if (FAILED(res))
	{
		std::cerr << "Failed to begin scene" << res << "\n";
		return TestResult::Other;
	}

	res = device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, uint32_t(0xFFFFFFFF), 1.0f, 0);
	if (FAILED(res))
	{
		std::cerr << "Failed to clear RT" << res << "\n";
		return TestResult::Other;
	}

	res = device->DrawIndexedPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 0, 3 * NUM_TRIS_TOTAL, 3, 1);
	if (FAILED(res))
	{
		std::cerr << "Failed to draw" << res << "\n";
		return TestResult::Other;
	}

	res = device->EndScene();
	if (FAILED(res))
	{
		std::cerr << "Failed to end scene" << res << "\n";
		return TestResult::Other;
	}

	res = device->Present(nullptr, nullptr, nullptr, nullptr);
	if (FAILED(res))
	{
		std::cerr << "Failed to present" << res << "\n";
		return TestResult::Other;
	}

	// Start the actual test
	res = WriteVertexBuffer(
		Position::Beginning,
		vboLockFlags,
		vboLockRangeMode,
		vboLockMode,
		vboWriteMode,
		vboUnlockMode
	);
	if (FAILED(res))
	{
		std::cerr << "Failed to write vertex buffer" << res << "\n";
		return TestResult::LockFailed;
	}
	res = WriteIndexBuffer(
		Position::Beginning,
		iboLockFlags,
		iboLockRangeMode,
		iboLockMode,
		iboWriteMode,
		iboUnlockMode
	);
	if (FAILED(res))
	{
		std::cerr << "Failed to write index buffer" << res << "\n";
		return TestResult::LockFailed;
	}

	res = device->BeginScene();
	if (FAILED(res))
	{
		std::cerr << "Failed to begin scene" << res << "\n";
		return TestResult::Other;
	}

	uint32_t firstIndex = 3;
	uint32_t firstVertex;
	uint32_t vertexCount;
	if (vbDawRangeMode == DrawRangeMode::All)
	{
		firstVertex = 0;
		vertexCount = 3 * NUM_TRIS_TOTAL;
	}
	else if (vbDawRangeMode == DrawRangeMode::IncorrectSubset)
	{
		firstVertex = 3 * 2;
		vertexCount = 3;
	}
	else
	{
		firstVertex = 3;
		vertexCount = 3;
	}

	res = device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, uint32_t(0xFFFFFFFF), 1.0f, 0);
	if (FAILED(res))
	{
		std::cerr << "Failed to clear RT" << res << "\n";
		return TestResult::Other;
	}

	res = WriteVertexBuffer(
		Position::BeforeDraw,
		vboLockFlags,
		vboLockRangeMode,
		vboLockMode,
		vboWriteMode,
		vboUnlockMode
	);
	if (FAILED(res))
	{
		std::cerr << "Failed to write vertex buffer" << res << "\n";
		return TestResult::LockFailed;
	}
	res = WriteIndexBuffer(
		Position::BeforeDraw,
		iboLockFlags,
		iboLockRangeMode,
		iboLockMode,
		iboWriteMode,
		iboUnlockMode
	);
	if (FAILED(res))
	{
		std::cerr << "Failed to write index buffer" << res << "\n";
		return TestResult::LockFailed;
	}

	res = device->DrawIndexedPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, firstVertex, vertexCount, firstIndex, 1);
	if (FAILED(res))
	{
		std::cerr << "Failed to draw" << res << "\n";
		return TestResult::Other;
	}

	res = WriteVertexBuffer(
		Position::AfterDraw,
		vboLockFlags,
		vboLockRangeMode,
		vboLockMode,
		vboWriteMode,
		vboUnlockMode
	);
	if (FAILED(res))
	{
		std::cerr << "Failed to write vertex buffer" << res << "\n";
		return TestResult::LockFailed;
	}
	res = WriteIndexBuffer(
		Position::AfterDraw,
		iboLockFlags,
		iboLockRangeMode,
		iboLockMode,
		iboWriteMode,
		iboUnlockMode
	);
	if (FAILED(res))
	{
		std::cerr << "Failed to write index buffer" << res << "\n";
		return TestResult::LockFailed;
	}

	ReadResult(Position::AfterDraw, readResultMode, result);
	res = device->EndScene();
	if (FAILED(res))
	{
		std::cerr << "Failed to end scene" << res << "\n";
		return TestResult::Other;
	}

	res = WriteVertexBuffer(
		Position::AfterScene,
		vboLockFlags,
		vboLockRangeMode,
		vboLockMode,
		vboWriteMode,
		vboUnlockMode
	);
	if (FAILED(res))
	{
		std::cerr << "Failed to write vertex buffer" << res << "\n";
		return TestResult::LockFailed;
	}
	res = WriteIndexBuffer(
		Position::AfterScene,
		iboLockFlags,
		iboLockRangeMode,
		iboLockMode,
		iboWriteMode,
		iboUnlockMode
	);
	if (FAILED(res))
	{
		std::cerr << "Failed to write index buffer" << res << "\n";
		return TestResult::LockFailed;
	}

	ReadResult(Position::AfterScene, readResultMode, result);

	res = device->Present(nullptr, nullptr, nullptr, nullptr);
	if (FAILED(res))
	{
		std::cerr << "Failed to present" << res << "\n";
		return TestResult::Other;
	}

	res = WriteVertexBuffer(
		Position::AfterFrame,
		vboLockFlags,
		vboLockRangeMode,
		vboLockMode,
		vboWriteMode,
		vboUnlockMode
	);
	if (FAILED(res))
	{
		std::cerr << "Failed to write vertex buffer" << res << "\n";
		return TestResult::LockFailed;
	}
	res = WriteIndexBuffer(
		Position::AfterFrame,
		iboLockFlags,
		iboLockRangeMode,
		iboLockMode,
		iboWriteMode,
		iboUnlockMode
	);
	if (FAILED(res))
	{
		std::cerr << "Failed to write index buffer" << res << "\n";
		return TestResult::LockFailed;
	}

	ReadResult(Position::AfterFrame, readResultMode, result);
}

HRESULT BufferLocking::WriteVertexBuffer(
	Position position,
	DWORD lockFlags,
	LockRangeMode lockRangeMode,
	LockMode lockMode,
	WriteMode writeMode,
	UnlockMode unlockMode
)
{
	HRESULT res = S_OK;
	uint32_t lockOffset;
	uint32_t lockLength;
	uint32_t pointerOffset;
	if (lockRangeMode == LockRangeMode::All)
	{
		lockLength = sizeof(Vertex4) * 3 * NUM_TRIS_TOTAL;
	}
	else if (lockRangeMode == LockRangeMode::None)
	{
		lockLength = 0;
	}
	else
	{
		lockLength = sizeof(Vertex4) * 3;
	}
	if (lockRangeMode == LockRangeMode::CorrectSubset)
	{
		lockOffset = sizeof(Vertex4) * 3;
		pointerOffset = 0;
	}
	else if (lockRangeMode == LockRangeMode::IncorrectSubset)
	{
		pointerOffset = 0;
		lockOffset = sizeof(Vertex4) * 3;
	}
	else
	{
		pointerOffset = sizeof(Vertex4) * 3 * 1;
		lockOffset = 0;
	}
	std::array<Vertex4, 3> newData = {
		-1.0f, -1.0f, 0.5f, 1.0f,    D3DCOLOR_ARGB(0xFF, 0, 0xFF, 0),
		-1.0f,  1.0f, 0.5f, 1.0f,    D3DCOLOR_ARGB(0xFF, 0, 0xFF, 0),
		 1.0f,  1.0f, 0.5f, 1.0f,    D3DCOLOR_ARGB(0xFF, 0, 0xFF, 0),
	};

	bool lock = (lockMode == LockMode::Beginning && position == Position::Beginning)
		|| (lockMode == LockMode::AfterDraw && position == Position::AfterDraw)
		|| (lockMode == LockMode::BeforeDraw && position == Position::BeforeDraw);
	bool write = (writeMode == WriteMode::AfterLock && lock)
		|| (writeMode == WriteMode::AfterDraw && position == Position::AfterDraw)
		|| (writeMode == WriteMode::AfterScene && position == Position::AfterScene)
		|| (writeMode == WriteMode::AfterFrame && position == Position::AfterFrame);
	bool unlock = (unlockMode == UnlockMode::AfterWrite && write)
		|| (unlockMode == UnlockMode::AfterLock && lock)
		|| (unlockMode == UnlockMode::AfterDraw && position == Position::AfterDraw)
		|| (unlockMode == UnlockMode::AfterScene && position == Position::AfterScene)
		|| (unlockMode == UnlockMode::AfterFrame && position == Position::AfterFrame);

	if (lock)
	{
		void* oldPtr = vbPtr;
		res = vb->Lock(lockOffset, lockLength, &vbPtr, lockFlags);
		if (FAILED(res))
		{
			std::cerr << "Failed to lock vertex buffer" << res << "\n";
			return res;
		}
		if (vbPtr != oldPtr)
		{
			std::cout << "Vertex buffer pointer changed" << res << "\n";
		}
	}

	if (write)
	{
		void* dst = reinterpret_cast<uint8_t*>(vbPtr) + pointerOffset;
		memcpy(dst, newData.data(), sizeof(newData));
	}

	if (unlock)
	{
		res = vb->Unlock();
		if (FAILED(res))
		{
			std::cerr << "Failed to unlock vertex buffer" << res << "\n";
			return res;
		}
	}

	if (!write && writeMode == WriteMode::AfterUnlock && unlock)
	{
		void* dst = reinterpret_cast<uint8_t*>(ibPtr) + pointerOffset;
		memcpy(ibPtr, newData.data(), sizeof(newData));
	}

	return res;
}

HRESULT BufferLocking::WriteIndexBuffer(
	Position position,
	DWORD lockFlags,
	LockRangeMode lockRangeMode,
	LockMode lockMode,
	WriteMode writeMode,
	UnlockMode unlockMode
)
{
	HRESULT res = S_OK;
	uint32_t lockOffset;
	uint32_t lockLength;
	uint32_t pointerOffset;
	if (lockRangeMode == LockRangeMode::All)
	{
		lockLength = sizeof(uint16_t) * 3 * NUM_TRIS_TOTAL;
	}
	else if (lockRangeMode == LockRangeMode::None)
	{
		lockLength = 0;
	}
	else
	{
		lockLength = sizeof(uint16_t) * 3;
	}
	if (lockRangeMode == LockRangeMode::CorrectSubset)
	{
		lockOffset = sizeof(uint16_t) * 3;
		pointerOffset = 0;
	}
	else if (lockRangeMode == LockRangeMode::IncorrectSubset)
	{
		pointerOffset = 0;
		lockOffset = sizeof(uint16_t) * 3 * 2;
	}
	else
	{
		pointerOffset = sizeof(uint16_t) * 3;
		lockOffset = 0;
	}
	std::array<uint16_t, 3> newData = { 3, 4, 5 };

	bool lock = (lockMode == LockMode::Beginning && position == Position::Beginning)
		|| (lockMode == LockMode::AfterDraw && position == Position::AfterDraw)
		|| (lockMode == LockMode::BeforeDraw && position == Position::BeforeDraw);
	bool write = (writeMode == WriteMode::AfterLock && lock)
		|| (writeMode == WriteMode::AfterDraw && position == Position::AfterDraw)
		|| (writeMode == WriteMode::AfterScene && position == Position::AfterScene)
		|| (writeMode == WriteMode::AfterFrame && position == Position::AfterFrame);
	bool unlock = (unlockMode == UnlockMode::AfterWrite && write)
		|| (unlockMode == UnlockMode::AfterLock && lock)
		|| (unlockMode == UnlockMode::AfterDraw && position == Position::AfterDraw)
		|| (unlockMode == UnlockMode::AfterScene && position == Position::AfterScene)
		|| (unlockMode == UnlockMode::AfterFrame && position == Position::AfterFrame);

	if (lock)
	{
		void* oldPtr = ibPtr;
		res = ib->Lock(lockOffset, lockLength, &ibPtr, lockFlags);
		if (FAILED(res))
		{
			std::cerr << "Failed to lock index buffer" << res << "\n";
			return res;
		}
		if (ibPtr != oldPtr)
		{
			std::cout << "Index buffer pointer changed" << res << "\n";
		}
	}

	if (write)
	{
		void* dst = reinterpret_cast<uint8_t*>(ibPtr) + pointerOffset;
		memcpy(ibPtr, newData.data(), sizeof(newData));
	}

	if (unlock)
	{
		res = ib->Unlock();
		if (FAILED(res))
		{
			std::cerr << "Failed to unlock index buffer" << res << "\n";
			return res;
		}
	}

	if (!write && writeMode == WriteMode::AfterUnlock && unlock)
	{
		void* dst = reinterpret_cast<uint8_t*>(ibPtr) + pointerOffset;
		memcpy(ibPtr, newData.data(), sizeof(newData));
	}

	return res;
}

HRESULT BufferLocking::ReadResult(
	Position position,
	ReadResultMode readMode,
	uint32_t* result
)
{
	HRESULT res = S_OK;

	if ((position == Position::AfterScene && readMode == ReadResultMode::NextScene)
		|| (position == Position::AfterFrame && readMode == ReadResultMode::NextFrame))
	{
		res = device->BeginScene();
		if (FAILED(res))
		{
			std::cerr << "Failed to begin scene" << res << "\n";
			return res;
		}
	}

	if ((position == Position::AfterDraw && readMode == ReadResultMode::AfterDraw)
		|| (position == Position::AfterScene && readMode == ReadResultMode::NextScene)
		|| (position == Position::AfterFrame && readMode == ReadResultMode::NextFrame))
	{
		dxvk::Com<IDirect3DSurface9> backbuffer;
		res = this->device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
		if (FAILED(res))
		{
			std::cerr << "Failed to retrieve backbuffer" << res << "\n";
			return res;
		}

		res = device->GetRenderTargetData(backbuffer.ptr(), surface.ptr());
		if (FAILED(res))
		{
			std::cerr << "Failed to get blit result" << res << "\n";
			return res;
		}

		D3DLOCKED_RECT locked;
		res = surface->LockRect(&locked, nullptr, 0);
		if (FAILED(res))
		{
			std::cerr << "Failed to lock blit result" << res << "\n";
			return res;
		}

		memcpy(result, locked.pBits, sizeof(uint32_t));

		res = surface->UnlockRect();
		if (FAILED(res))
		{
			std::cerr << "Failed to unlock blit result" << res << "\n";
			return res;
		}
	}

	if ((position == Position::AfterScene && readMode == ReadResultMode::NextScene)
		|| (position == Position::AfterFrame && readMode == ReadResultMode::NextFrame))
	{
		res = device->EndScene();
		if (FAILED(res))
		{
			std::cerr << "Failed to end scene" << res << "\n";
			return res;
		}
	}

	return res;
}
