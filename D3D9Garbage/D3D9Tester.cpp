#include "D3D9Tester.h"
#include "StateBlockLeakResetTest.h"
#include "ATIATOC.h"

#include <array>

const Test SELECTED_TEST = Test::ATIATOC;

D3D9Tester::D3D9Tester(HWND window) {
	dxvk::Com<IDirect3D9> d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS params = {
		1280, 720, D3DFMT_UNKNOWN, 3, D3DMULTISAMPLE_NONE, 0, D3DSWAPEFFECT_DISCARD, window, true, false, D3DFMT_UNKNOWN, 0, 0, D3DPRESENT_INTERVAL_DEFAULT
	};
	dxvk::Com<IDirect3DDevice9> device;
	d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, nullptr, D3DCREATE_FPU_PRESERVE | D3DCREATE_MULTITHREADED | D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &device);

	switch (SELECTED_TEST)
	{
	case Test::StateBlockLeakReset:
		this->test = std::make_unique<StateBlockLeakResetTest>(window, std::move(d3d9), std::move(device));
		break;

	case Test::ATIATOC:
		this->test = std::make_unique<ATIATOC>(window, std::move(d3d9), std::move(device));
		break;
	}
}

void D3D9Tester::Render() {
	if (test != nullptr)
		test->Render();
}
