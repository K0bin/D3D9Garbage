#pragma once

#include "D3D9Test.h"

class LockableMSAATest : public D3D9Test
{
public:
	LockableMSAATest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:
};

