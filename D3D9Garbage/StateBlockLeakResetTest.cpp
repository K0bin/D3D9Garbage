#include "StateBlockLeakResetTest.h"

#include <array>

StateBlockLeakResetTest::StateBlockLeakResetTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device)) {
}

void StateBlockLeakResetTest::Render() {
	device->BeginScene();
	D3DRECT rect = {
		0, 0, 1280, 720
	};

	device->BeginStateBlock();
	std::array<float, 4> testConst = { 10.0f, 11.0f, 12.0f, 13.0f };
	device->SetPixelShaderConstantF(0, testConst.data(), 1);

	dxvk::Com<IDirect3DStateBlock9> sb;
	HRESULT res = device->EndStateBlock(&sb);
	//sb = nullptr;

	device->BeginStateBlock();
	device->SetPixelShaderConstantF(0, testConst.data(), 1);

	dxvk::Com<IDirect3DStateBlock9> sb2;
	res = device->EndStateBlock(&sb2);
	sb2 = nullptr;

	D3DPRESENT_PARAMETERS params = {
	1280, 720, D3DFMT_UNKNOWN, 1, D3DMULTISAMPLE_NONE, 0, D3DSWAPEFFECT_DISCARD, window, true, false, D3DFMT_UNKNOWN, 0, 0, 1
	};
	HRESULT resetRes = device->Reset(&params);

	device->Clear(1, &rect, D3DCLEAR_TARGET, 0xFF00FFAA, 0.0f, 0);
	device->EndScene();
	device->Present(nullptr, nullptr, nullptr, nullptr);
}
