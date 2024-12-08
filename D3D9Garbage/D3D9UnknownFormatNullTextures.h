#pragma once

#include "D3D9Test.h"

class D3D9UnknownFormatNullTextures : public D3D9Test
{
public:
	D3D9UnknownFormatNullTextures(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:


};

