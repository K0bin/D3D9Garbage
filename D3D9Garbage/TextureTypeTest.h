#pragma once

#include "D3D9Test.h"

struct TextureTypeVertex {
	float x; float y; float z; float w;
	float texCoordX; float texCoordY; float texCoordZ; float texCoordW;
};

class TextureTypeTest : public D3D9Test {
public:
	TextureTypeTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device);
	void Render() override;

private:
	dxvk::Com<IDirect3DVertexBuffer9> vb;
	dxvk::Com<IDirect3DIndexBuffer9> ib;
	dxvk::Com<IDirect3DVolumeTexture9> texture3d;
	dxvk::Com<IDirect3DCubeTexture9> textureCube;
	dxvk::Com<IDirect3DTexture9> texture2d;
	dxvk::Com<IDirect3DPixelShader9> ps;
	dxvk::Com<IDirect3DPixelShader9> ps_3d;
	dxvk::Com<IDirect3DPixelShader9> ps_cube;
	dxvk::Com<IDirect3DPixelShader9> ps_3;
	dxvk::Com<IDirect3DPixelShader9> ps_3d_3;
	dxvk::Com<IDirect3DPixelShader9> ps_cube_3;
	dxvk::Com<IDirect3DVertexShader9> vs;
	dxvk::Com<IDirect3DVertexShader9> vs_3;
	dxvk::Com<IDirect3DVertexDeclaration9> vertexDecl;

};