#pragma once

#include "com_ptr.h"
#include <memory>
#include <string_view>
#include <vector>

#define D3D_DEBUG_INFO
#include <d3d9.h>

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |       \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */


class D3D9Test {
public:
	D3D9Test(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	virtual void Render();

protected:
	HWND window;
	dxvk::Com<IDirect3D9> d3d9;
	dxvk::Com<IDirect3DDevice9> device;
	std::vector<uint8_t> readFile(const std::string_view& file);
};
