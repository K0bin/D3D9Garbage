#include "NvAPISLI.h"

#define __NVAPI_EMPTY_SAL
#include "NvAPI/nvapi.h"
#undef __NVAPI_EMPTY_SAL

#include "NvAPI/nvapi_interface.h"

#include <ctype.h>
#include <iostream>

#include "d3d11.h"

uint32_t FindInterface(const char* name) {
	for (const auto& pair : nvapi_interface_table) {
		if (pair.func == name) {
			return pair.id;
		}
	}
	return 0;
}

NVAPISLITest::NVAPISLITest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device)) {

	HMODULE nvapi = LoadLibrary(L"nvapi64.dll");
	if (nvapi == nullptr) {
		std::cout << "Cannot load NVAPI" << std::endl;
		return;
	}

	auto query = reinterpret_cast<void* (*)(NvU32)>(GetProcAddress(nvapi, "nvapi_QueryInterface"));
	if (query == nullptr) {
		std::cout << "Query is null" << std::endl;
		return;
	}

	auto init = reinterpret_cast<decltype(&NvAPI_Initialize)>(query(FindInterface("NvAPI_Initialize")));
	if (init == nullptr) {
		std::cout << "Init is null" << std::endl;
		return;
	}

	auto get_sli_state = reinterpret_cast<decltype(&NvAPI_D3D_GetCurrentSLIState)>(query(FindInterface("NvAPI_D3D_GetCurrentSLIState")));
	if (get_sli_state == nullptr) {
		std::cout << "NvAPI_D3D_GetCurrentSLIState is null" << std::endl;
		return;
	}

	auto implicit_sli_control = reinterpret_cast<decltype(&NvAPI_D3D_ImplicitSLIControl)>(query(FindInterface("NvAPI_D3D_ImplicitSLIControl")));
	if (implicit_sli_control == nullptr) {
		std::cout << "NvAPI_D3D_ImplicitSLIControl is null" << std::endl;
		return;
	}

	NvAPI_Status res = init();
	if (res != NVAPI_OK) {
		std::cout << "Init failed: " << res << std::endl;
		return;
	}

	HRESULT hr;
	
	NV_GET_CURRENT_SLI_STATE state;
	state.version = NV_GET_CURRENT_SLI_STATE_VER;
	state.bIsCurAFRGroupNew = 0xdeadbeef;
	state.currentAFRIndex = 0xdeadbeef;
	state.maxNumAFRGroups = 0xdeadbeef;
	state.nextFrameAFRIndex = 0xdeadbeef;
	state.numAFRGroups = 0xdeadbeef;
	state.numVRSLIGpus = 0xdeadbeef;
	state.previousFrameAFRIndex = 0xdeadbeef;
	res = get_sli_state(this->device.ptr(), &state);
	if (res == NVAPI_OK) {
		std::cout << "bIsCurAFRGroupNew: " << state.bIsCurAFRGroupNew << std::endl;
		std::cout << "currentAFRIndex: " << state.currentAFRIndex << std::endl;
		std::cout << "maxNumAFRGroups: " << state.maxNumAFRGroups << std::endl;
		std::cout << "nextFrameAFRIndex: " << state.nextFrameAFRIndex << std::endl;
		std::cout << "numAFRGroups: " << state.numAFRGroups << std::endl;
		std::cout << "numVRSLIGpus: " << state.numVRSLIGpus << std::endl;
		std::cout << "previousFrameAFRIndex: " << state.previousFrameAFRIndex << std::endl;
		std::cout << "version: " << state.version << std::endl;
	}
	else {
		std::cout << "NvAPI_D3D_GetCurrentSLIState failed: " << res << std::endl;
	}

	state.version = NV_GET_CURRENT_SLI_STATE_VER1;
	state.bIsCurAFRGroupNew = 0xdeadbeef;
	state.currentAFRIndex = 0xdeadbeef;
	state.maxNumAFRGroups = 0xdeadbeef;
	state.nextFrameAFRIndex = 0xdeadbeef;
	state.numAFRGroups = 0xdeadbeef;
	state.numVRSLIGpus = 0xdeadbeef;
	state.previousFrameAFRIndex = 0xdeadbeef;
	res = get_sli_state(this->device.ptr(), &state);
	if (res == NVAPI_OK) {
		std::cout << "V1 bIsCurAFRGroupNew: " << state.bIsCurAFRGroupNew << std::endl;
		std::cout << "V1 currentAFRIndex: " << state.currentAFRIndex << std::endl;
		std::cout << "V1 maxNumAFRGroups: " << state.maxNumAFRGroups << std::endl;
		std::cout << "V1 nextFrameAFRIndex: " << state.nextFrameAFRIndex << std::endl;
		std::cout << "V1 numAFRGroups: " << state.numAFRGroups << std::endl;
		std::cout << "V1 previousFrameAFRIndex: " << state.previousFrameAFRIndex << std::endl;
		std::cout << "V1 version: " << state.version << std::endl;
	}
	else {
		std::cout << "V1 NvAPI_D3D_GetCurrentSLIState failed: " << res << std::endl;
	}

	state.version = NV_GET_CURRENT_SLI_STATE_VER;
	state.bIsCurAFRGroupNew = 0xdeadbeef;
	state.currentAFRIndex = 0xdeadbeef;
	state.maxNumAFRGroups = 0xdeadbeef;
	state.nextFrameAFRIndex = 0xdeadbeef;
	state.numAFRGroups = 0xdeadbeef;
	state.numVRSLIGpus = 0xdeadbeef;
	state.previousFrameAFRIndex = 0xdeadbeef;
	res = get_sli_state(nullptr, &state);
	if (res != NVAPI_OK) {
		std::cout << "NvAPI_D3D_GetCurrentSLIState with null device failed: " << res << std::endl;
	}
	state.version = NV_GET_CURRENT_SLI_STATE_VER;
	state.bIsCurAFRGroupNew = 0xdeadbeef;
	state.currentAFRIndex = 0xdeadbeef;
	state.maxNumAFRGroups = 0xdeadbeef;
	state.nextFrameAFRIndex = 0xdeadbeef;
	state.numAFRGroups = 0xdeadbeef;
	state.numVRSLIGpus = 0xdeadbeef;
	state.previousFrameAFRIndex = 0xdeadbeef;
	res = get_sli_state(this->device.ptr(), nullptr);
	if (res != NVAPI_OK) {
		std::cout << "NvAPI_D3D_GetCurrentSLIState with null state failed: " << res << std::endl;
	}
	state.version = NV_GET_CURRENT_SLI_STATE_VER;
	state.bIsCurAFRGroupNew = 0xdeadbeef;
	state.currentAFRIndex = 0xdeadbeef;
	state.maxNumAFRGroups = 0xdeadbeef;
	state.nextFrameAFRIndex = 0xdeadbeef;
	state.numAFRGroups = 0xdeadbeef;
	state.numVRSLIGpus = 0xdeadbeef;
	state.previousFrameAFRIndex = 0xdeadbeef;
	res = get_sli_state(nullptr, nullptr);
	if (res != NVAPI_OK) {
		std::cout << "NvAPI_D3D_GetCurrentSLIState with null device and null state failed: " << res << std::endl;
	}
	state.version = 0xdeadbeef;
	state.bIsCurAFRGroupNew = 0xdeadbeef;
	state.currentAFRIndex = 0xdeadbeef;
	state.maxNumAFRGroups = 0xdeadbeef;
	state.nextFrameAFRIndex = 0xdeadbeef;
	state.numAFRGroups = 0xdeadbeef;
	state.numVRSLIGpus = 0xdeadbeef;
	state.previousFrameAFRIndex = 0xdeadbeef;
	res = get_sli_state(this->device.ptr(), &state);
	if (res != NVAPI_OK) {
		std::cout << "NvAPI_D3D_GetCurrentSLIState with bullshit version failed: " << res << std::endl;
	}

	res = implicit_sli_control(ENABLE_IMPLICIT_SLI);
	if (res != NVAPI_OK) {
		std::cout << "NvAPI_D3D_ImplicitSLIControl (Enable) failed: " << res << std::endl;
	}
	res = implicit_sli_control(DISABLE_IMPLICIT_SLI);
	if (res != NVAPI_OK) {
		std::cout << "NvAPI_D3D_ImplicitSLIControl (Disable) failed: " << res << std::endl;
	}

	state.version = NV_GET_CURRENT_SLI_STATE_VER;
	state.bIsCurAFRGroupNew = 0xdeadbeef;
	state.currentAFRIndex = 0xdeadbeef;
	state.maxNumAFRGroups = 0xdeadbeef;
	state.nextFrameAFRIndex = 0xdeadbeef;
	state.numAFRGroups = 0xdeadbeef;
	state.numVRSLIGpus = 0xdeadbeef;
	state.previousFrameAFRIndex = 0xdeadbeef;
	res = get_sli_state(this->d3d9.ptr(), &state);
	if (res == NVAPI_OK) {
		std::cout << "bIsCurAFRGroupNew: " << state.bIsCurAFRGroupNew << std::endl;
		std::cout << "currentAFRIndex: " << state.currentAFRIndex << std::endl;
		std::cout << "maxNumAFRGroups: " << state.maxNumAFRGroups << std::endl;
		std::cout << "nextFrameAFRIndex: " << state.nextFrameAFRIndex << std::endl;
		std::cout << "numAFRGroups: " << state.numAFRGroups << std::endl;
		std::cout << "numVRSLIGpus: " << state.numVRSLIGpus << std::endl;
		std::cout << "previousFrameAFRIndex: " << state.previousFrameAFRIndex << std::endl;
		std::cout << "version: " << state.version << std::endl;
	}
	else {
		std::cout << "NvAPI_D3D_GetCurrentSLIState failed: " << res << std::endl;
	}

	dxvk::Com<IDirect3DVertexBuffer9> buffer;
	hr = this->device->CreateVertexBuffer(256, 0, 0, D3DPOOL_DEFAULT, &buffer, nullptr);

	state.version = NV_GET_CURRENT_SLI_STATE_VER;
	state.bIsCurAFRGroupNew = 0xdeadbeef;
	state.currentAFRIndex = 0xdeadbeef;
	state.maxNumAFRGroups = 0xdeadbeef;
	state.nextFrameAFRIndex = 0xdeadbeef;
	state.numAFRGroups = 0xdeadbeef;
	state.numVRSLIGpus = 0xdeadbeef;
	state.previousFrameAFRIndex = 0xdeadbeef;
	res = get_sli_state(buffer.ptr(), &state);
	if (res == NVAPI_OK) {
		std::cout << "bIsCurAFRGroupNew: " << state.bIsCurAFRGroupNew << std::endl;
		std::cout << "currentAFRIndex: " << state.currentAFRIndex << std::endl;
		std::cout << "maxNumAFRGroups: " << state.maxNumAFRGroups << std::endl;
		std::cout << "nextFrameAFRIndex: " << state.nextFrameAFRIndex << std::endl;
		std::cout << "numAFRGroups: " << state.numAFRGroups << std::endl;
		std::cout << "numVRSLIGpus: " << state.numVRSLIGpus << std::endl;
		std::cout << "previousFrameAFRIndex: " << state.previousFrameAFRIndex << std::endl;
		std::cout << "version: " << state.version << std::endl;
	}
	else {
		std::cout << "NvAPI_D3D_GetCurrentSLIState failed: " << res << std::endl;
	}

	dxvk::Com<ID3D11Device> d3d11Device;
	dxvk::Com<ID3D11DeviceContext> d3d11Context;
	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &d3d11Device, nullptr, &d3d11Context);
	state.version = NV_GET_CURRENT_SLI_STATE_VER;
	state.bIsCurAFRGroupNew = 0xdeadbeef;
	state.currentAFRIndex = 0xdeadbeef;
	state.maxNumAFRGroups = 0xdeadbeef;
	state.nextFrameAFRIndex = 0xdeadbeef;
	state.numAFRGroups = 0xdeadbeef;
	state.numVRSLIGpus = 0xdeadbeef;
	state.previousFrameAFRIndex = 0xdeadbeef;
	res = get_sli_state(d3d11Device.ptr(), &state);
	if (res == NVAPI_OK) {
		std::cout << "D3D11 bIsCurAFRGroupNew: " << state.bIsCurAFRGroupNew << std::endl;
		std::cout << "D3D11 currentAFRIndex: " << state.currentAFRIndex << std::endl;
		std::cout << "D3D11 maxNumAFRGroups: " << state.maxNumAFRGroups << std::endl;
		std::cout << "D3D11 nextFrameAFRIndex: " << state.nextFrameAFRIndex << std::endl;
		std::cout << "D3D11 numAFRGroups: " << state.numAFRGroups << std::endl;
		std::cout << "D3D11 numVRSLIGpus: " << state.numVRSLIGpus << std::endl;
		std::cout << "D3D11 previousFrameAFRIndex: " << state.previousFrameAFRIndex << std::endl;
		std::cout << "D3D11 version: " << state.version << std::endl;
	}
	else {
		std::cout << "D3D11 NvAPI_D3D_GetCurrentSLIState failed: " << res << std::endl;
	}

	state.version = NV_GET_CURRENT_SLI_STATE_VER;
	state.bIsCurAFRGroupNew = 0xdeadbeef;
	state.currentAFRIndex = 0xdeadbeef;
	state.maxNumAFRGroups = 0xdeadbeef;
	state.nextFrameAFRIndex = 0xdeadbeef;
	state.numAFRGroups = 0xdeadbeef;
	state.numVRSLIGpus = 0xdeadbeef;
	state.previousFrameAFRIndex = 0xdeadbeef;
	res = get_sli_state(d3d11Context.ptr(), &state);
	if (res == NVAPI_OK) {
		std::cout << "D3D11 bIsCurAFRGroupNew: " << state.bIsCurAFRGroupNew << std::endl;
		std::cout << "D3D11 currentAFRIndex: " << state.currentAFRIndex << std::endl;
		std::cout << "D3D11 maxNumAFRGroups: " << state.maxNumAFRGroups << std::endl;
		std::cout << "D3D11 nextFrameAFRIndex: " << state.nextFrameAFRIndex << std::endl;
		std::cout << "D3D11 numAFRGroups: " << state.numAFRGroups << std::endl;
		std::cout << "D3D11 numVRSLIGpus: " << state.numVRSLIGpus << std::endl;
		std::cout << "D3D11 previousFrameAFRIndex: " << state.previousFrameAFRIndex << std::endl;
		std::cout << "D3D11 version: " << state.version << std::endl;
	}
	else {
		std::cout << "D3D11 NvAPI_D3D_GetCurrentSLIState failed: " << res << std::endl;
	}

	dxvk::Com<ID3D11Buffer> d3d11Buffer;
	D3D11_BUFFER_DESC d3d11BufferDesc = {};
	d3d11BufferDesc.ByteWidth = 256;
	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	hr = d3d11Device->CreateBuffer(&d3d11BufferDesc, nullptr, &d3d11Buffer);
	state.version = NV_GET_CURRENT_SLI_STATE_VER;
	state.bIsCurAFRGroupNew = 0xdeadbeef;
	state.currentAFRIndex = 0xdeadbeef;
	state.maxNumAFRGroups = 0xdeadbeef;
	state.nextFrameAFRIndex = 0xdeadbeef;
	state.numAFRGroups = 0xdeadbeef;
	state.numVRSLIGpus = 0xdeadbeef;
	state.previousFrameAFRIndex = 0xdeadbeef;
	res = get_sli_state(d3d11Context.ptr(), &state);
	if (res == NVAPI_OK) {
		std::cout << "D3D11 bIsCurAFRGroupNew: " << state.bIsCurAFRGroupNew << std::endl;
		std::cout << "D3D11 currentAFRIndex: " << state.currentAFRIndex << std::endl;
		std::cout << "D3D11 maxNumAFRGroups: " << state.maxNumAFRGroups << std::endl;
		std::cout << "D3D11 nextFrameAFRIndex: " << state.nextFrameAFRIndex << std::endl;
		std::cout << "D3D11 numAFRGroups: " << state.numAFRGroups << std::endl;
		std::cout << "D3D11 numVRSLIGpus: " << state.numVRSLIGpus << std::endl;
		std::cout << "D3D11 previousFrameAFRIndex: " << state.previousFrameAFRIndex << std::endl;
		std::cout << "D3D11 version: " << state.version << std::endl;
	}
	else {
		std::cout << "D3D11 NvAPI_D3D_GetCurrentSLIState failed: " << res << std::endl;
	}


	std::cout << "Bye." << std::endl;
}

void NVAPISLITest::Render() {}