#include "D3D9Tester.h"
#include "StateBlockLeakResetTest.h"
#include "ATIATOC.h"
#include "NullStreamTest.h"
#include "FFTest.h"
#include "FFTexcoordTest.h"
#include "StateBlockVertexOffsetTest.h"
#include "BufferLocking.h"
#include "StretchRectTest.h"

#include <array>

const Test SELECTED_TEST = Test::StretchRect;

D3D9Tester::D3D9Tester(HWND window) {
	dxvk::Com<IDirect3D9> d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	dxvk::Com<IDirect3DDevice9> device;

	D3DPRESENT_PARAMETERS params = {
		640, 480, D3DFMT_A8R8G8B8, 1, D3DMULTISAMPLE_NONE, 0, D3DSWAPEFFECT_DISCARD, window, true, true, D3DFMT_D24S8, 0, 0, D3DPRESENT_INTERVAL_DEFAULT
	};
	HRESULT res = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, nullptr, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &device);

	/*D3DPRESENT_PARAMETERS params = {
		640, 480, D3DFMT_A8R8G8B8, 0, D3DMULTISAMPLE_NONE, 0, D3DSWAPEFFECT_DISCARD, nullptr, true, true, D3DFMT_D24S8, 0, 0, D3DPRESENT_INTERVAL_DEFAULT
	};
	HRESULT res = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, nullptr, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &device);*/

	switch (SELECTED_TEST)
	{
	case Test::StateBlockLeakReset:
		this->test = std::make_unique<StateBlockLeakResetTest>(window, std::move(d3d9), std::move(device));
		break;

	case Test::ATIATOC:
		this->test = std::make_unique<ATIATOC>(window, std::move(d3d9), std::move(device));
		break;

	case Test::NullStream:
		this->test = std::make_unique<NullStreamTest>(window, std::move(d3d9), std::move(device));
		break;

	case Test::FixedFunction:
		this->test = std::make_unique<FFTest>(window, std::move(d3d9), std::move(device));
		break;

	case Test::StateBlockVertexOffset:
		this->test = std::make_unique<StateBlockVertexOffsetTest>(window, std::move(d3d9), std::move(device));
		break;

	case Test::FixedFunctionTexcoord:
		this->test = std::make_unique<FFTexcoordTest>(window, std::move(d3d9), std::move(device));
		break;

	case Test::BufferLocking:
		this->test = std::make_unique<BufferLocking>(window, std::move(d3d9), std::move(device));
		break;

	case Test::StretchRect:
		this->test = std::make_unique<StretchRectTest>(window, std::move(d3d9), std::move(device));
		break;
	}
}

void D3D9Tester::Render() {
	if (test != nullptr)
		test->Render();
}
