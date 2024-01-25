#pragma once

#include "com_ptr.h"
#include <memory>

#define D3D_DEBUG_INFO
#include <d3d9.h>

#include "D3D9Test.h"

class D3D9Tester {
public:
	D3D9Tester(HWND window);
	void Render();

private:
	std::unique_ptr<D3D9Test> test;
};

enum class Test {
	StateBlockLeakReset,
	ATIATOC
};

