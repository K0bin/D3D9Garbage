#pragma once

#include "D3D9Test.h"

enum class LockMode {
	Beginning,
	BeforeDraw,
	AfterDraw,
};

enum class UnlockMode {
	AfterLock,
	AfterWrite,
	BeforeDraw,
	AfterDraw,
	AfterScene,
	AfterFrame,
	Never
};

enum class WriteMode {
	AfterLock,
	AfterUnlock,
	AfterDraw,
	AfterScene,
	AfterFrame,
	Never
};

enum class ReadResultMode {
	AfterDraw,
	NextScene,
	NextFrame
};

enum class DrawRangeMode {
	All,
	IncorrectSubset,
	CorrectSubset
};

enum class LockRangeMode {
	None,
	IncorrectSubset,
	CorrectSubset,
	All
};

enum class Position {
	Beginning,
	BeforeDraw,
	AfterDraw,
	AfterScene,
	AfterFrame
};

enum class TestResult : uint32_t {
	Success = 0,
	CreationFailed,
	LockFailed,
	Other
};


class BufferLocking : public D3D9Test {

public:
	BufferLocking(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:
	bool TestBufferVariants(
		D3DPOOL iboPool,
		DWORD iboUsages,
		D3DPOOL vboPool,
		DWORD vboUsages
	);

	HRESULT CreateSurface();
	HRESULT CreateBuffers(
		D3DPOOL iboPool,
		DWORD iboUsages,
		D3DPOOL vboPool,
		DWORD vboUsages
	);
	HRESULT PrefillBuffers();
	TestResult Test(
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
	);

	HRESULT WriteVertexBuffer(
		Position position,
		DWORD lockFlags,
		LockRangeMode lockRangeMode,
		LockMode lockMode,
		WriteMode writeMode,
		UnlockMode unlockMode
	);

	HRESULT WriteIndexBuffer(
		Position position,
		DWORD lockFlags,
		LockRangeMode lockRangeMode,
		LockMode lockMode,
		WriteMode writeMode,
		UnlockMode unlockMode
	);

	HRESULT ReadResult(
		Position position,
		ReadResultMode readMode,
		uint32_t* result
	);

	dxvk::Com<IDirect3DVertexShader9> vs;
	dxvk::Com<IDirect3DPixelShader9> ps;
	dxvk::Com<IDirect3DVertexBuffer9> vb;
	dxvk::Com<IDirect3DIndexBuffer9> ib;
	dxvk::Com<IDirect3DVertexDeclaration9> vertexDecl;
	dxvk::Com<IDirect3DSurface9> surface;

	void* vbPtr;
	void* ibPtr;

	uint64_t frame = 0;

};


