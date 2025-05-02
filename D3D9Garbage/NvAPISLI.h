#pragma once

#include <string_view>
#include <vector>

#include "D3D9Test.h"

class NVAPISLITest : public D3D9Test {
public:
	NVAPISLITest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:
};

