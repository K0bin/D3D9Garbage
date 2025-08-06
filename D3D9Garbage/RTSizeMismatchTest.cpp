#include "RTSizeMismatchTest.h"

#include <array>

RTSizeMismatchTest::RTSizeMismatchTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
    : D3D9Test(window, std::move(d3d9), std::move(device))
{
    IDirect3DDevice9Ex* deviceEx = static_cast<IDirect3DDevice9Ex*>(device.ptr());

    HRESULT res;

    D3DCAPS9 caps;
    res = d3d9->GetDeviceCaps(0, D3DDEVTYPE_HAL, &caps);

    D3DFORMAT nullFmt = static_cast<D3DFORMAT>(MAKEFOURCC('N', 'U', 'L', 'L'));

    res = device->CreateDepthStencilSurface(640, 480, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, false, &this->ds, nullptr);
    res = device->CreateDepthStencilSurface(640, 480, D3DFMT_D24X8, D3DMULTISAMPLE_4_SAMPLES, 0, false, &this->dsMS, nullptr);
    res = device->CreateDepthStencilSurface(800, 600, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, false, &this->dsLarge, nullptr);
    res = device->CreateDepthStencilSurface(320, 240, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, false, &this->dsSmall, nullptr);
    res = device->CreateRenderTarget(640, 480, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &this->rt, nullptr);
    res = device->CreateRenderTarget(640, 480, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_4_SAMPLES, 0, false, &this->rtMS, nullptr);
    res = device->CreateRenderTarget(800, 600, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &this->rtLarge, nullptr);
    res = device->CreateRenderTarget(640, 480, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &this->rt1, nullptr);
    res = device->CreateRenderTarget(640, 480, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &this->rt2, nullptr);
    res = device->CreateRenderTarget(320, 240, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &this->rtSmall, nullptr);
    //res = device->CreateRenderTarget(640, 480, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &this->rtSmall1, nullptr);
    //res = device->CreateRenderTarget(1, 1, nullFmt, D3DMULTISAMPLE_NONE, 0, false, &this->rtSmall1, nullptr);
    //res = device->CreateRenderTarget(1, 1, nullFmt, D3DMULTISAMPLE_4_SAMPLES, 0, false, &this->rtSmall1, nullptr);
    //res = device->CreateRenderTarget(640, 480, nullFmt, D3DMULTISAMPLE_NONE, 0, false, &this->rtSmall1, nullptr);
    //res = device->CreateRenderTarget(640, 480, nullFmt, D3DMULTISAMPLE_4_SAMPLES, 0, false, &this->rtSmall1, nullptr);
    res = device->CreateRenderTarget(64, 64, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &this->rtSmall2, nullptr);
    //res = device->CreateRenderTarget(1, 1, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &this->rtSmall3, nullptr);
    res = device->CreateRenderTarget(2, 1, nullFmt, D3DMULTISAMPLE_NONE, 0, false, &this->rtSmall3, nullptr);
    //res = device->CreateRenderTarget(1, 1, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_4_SAMPLES, 0, false, &this->rtSmall3, nullptr);
    //res = device->CreateRenderTarget(4, 4, nullFmt, D3DMULTISAMPLE_4_SAMPLES, 0, false, &this->rtSmall3, nullptr);

    res = device->SetDepthStencilSurface(this->ds.ptr());
    D3DVIEWPORT9 viewport = {
        0, 0, 1280, 720
    };
    res = device->SetViewport(&viewport);
    res = device->Clear(0, nullptr, D3DCLEAR_ZBUFFER, 0, 1.0, 0);
    res = device->SetDepthStencilSurface(this->dsSmall.ptr());
    viewport = {
        0, 0, 1280, 720
    };
    res = device->SetViewport(&viewport);
    res = device->GetViewport(&viewport);
    res = device->Clear(0, nullptr, D3DCLEAR_ZBUFFER, 0, 1.0, 0);

}

void RTSizeMismatchTest::Render()
{
    HRESULT res;

    dxvk::Com<IDirect3DSurface9> backbuffer;
    res = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
    res = device->SetRenderTarget(0, backbuffer.ptr());

    D3DSURFACE_DESC surfDesc;
    res = backbuffer->GetDesc(&surfDesc);
    res = this->ds->GetDesc(&surfDesc);

    //res = device->SetRenderTarget(0, rt.ptr());
    res = device->SetDepthStencilSurface(ds.ptr());
    //res = device->SetDepthStencilSurface(dsLarge.ptr());
    //res = device->SetDepthStencilSurface(dsSmall.ptr());

    res = device->SetRenderState(D3DRS_SCISSORTESTENABLE, 0);
    res = device->SetRenderState(D3DRS_ZENABLE, 1);
    res = device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
    res = device->SetRenderState(D3DRS_ZWRITEENABLE, 1);
    //res = device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    res = device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    res = device->SetVertexShader(nullptr);
    res = device->SetPixelShader(nullptr);

    D3DVIEWPORT9 viewport = {
        0, 0, 1280, 720, 0.0f, 1.0f
    };
    res = device->SetViewport(&viewport);
    RECT rect = { 0, 0, 1280, 720 };
    res = device->SetScissorRect(&rect);

    res = device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
    dxvk::Com<IDirect3DVertexDeclaration9> vertexDecl;
    res = device->GetVertexDeclaration(&vertexDecl);
    uint32_t count;
    std::array<D3DVERTEXELEMENT9, 16> elements;
    D3DVERTEXELEMENT9 element;
    res = vertexDecl->GetDeclaration(elements.data(), &count);

    res = device->BeginScene();

    res = device->GetViewport(&viewport);


    //res = this->rtSmall->GetDesc(&surfDesc);
    viewport.Width = surfDesc.Width;
    viewport.Height = surfDesc.Height;
    //res = device->SetViewport(&viewport);
    res = device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
    res = device->SetRenderState(D3DRS_COLORWRITEENABLE1, 0);




    device->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFF);
    D3DMATERIAL9 mtrl = {};
    mtrl.Ambient.r = 1.0;
    mtrl.Ambient.g = 1.0;
    mtrl.Ambient.b = 1.0;
    mtrl.Ambient.a = 1.0;
    device->SetMaterial(&mtrl);

    //res = device->SetRenderTarget(0, nullptr);
    //res = device->SetRenderTarget(0, rt.ptr());
    //res = device->SetRenderTarget(1, backbuffer.ptr());
    //res = device->SetRenderTarget(1, rt.ptr());
    //res = device->SetRenderTarget(1, rtSmall.ptr());
    //res = device->SetRenderTarget(1, rtLarge.ptr());
    //res = device->SetRenderTarget(0, backbuffer.ptr());
    //res = device->Clear(0, nullptr, D3DCLEAR_TARGET, 0xFF0000FF, 0.0, 0);
    res = device->SetRenderTarget(0, backbuffer.ptr());
    res = device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF00FFFF, 1.0, 0);
    res = device->TestCooperativeLevel();

    const uint32_t col = 0xFFFFFFFF;
    std::array<float, 3 * 8> verts = {
        -1.0f, -1.0f, 0.5f, *reinterpret_cast<const float*>(&col),
        -1.0f, 1.0f, 0.5f, *reinterpret_cast<const float*>(&col),
        1.0f, 1.0f, 0.5f, *reinterpret_cast<const float*>(&col),
    };
    res = device->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
    res = device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
    res = device->SetRenderTarget(0, rtSmall2.ptr());
    res = device->SetRenderTarget(1, nullptr);
    //res = device->SetRenderTarget(0, rt.ptr());
    //res = device->SetRenderState(D3DRS_ZENABLE, 0);
    res = device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    //res = device->SetRenderState(D3DRS_ZWRITEENABLE, 1);
    res = device->SetRenderState(D3DRS_ALPHATESTENABLE, 0);
    res = device->SetRenderState(D3DRS_ALPHAREF, 0);
    res = device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    res = device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
    res = device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, 0);
    res = device->SetDepthStencilSurface(ds.ptr());

    res = device->GetViewport(&viewport);
    res = device->GetScissorRect(&rect);

    res = device->SetRenderTarget(0, rtSmall2.ptr());
    RECT scissor = { 0, 0, 640, 480 };
    //res = device->SetScissorRect(&rect);
    viewport.Width = 640;
    viewport.Height = 480;
    res = device->SetViewport(&viewport);
    res = device->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
    res = device->SetRenderState(D3DRS_COLORWRITEENABLE1, 0);
    //res = device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
    //res = device->SetRenderState(D3DRS_COLORWRITEENABLE1, 0xFFFFFFFF);
    res = device->GetViewport(&viewport);
    //res = device->SetRenderTarget(0, rtMS.ptr());
    //res = device->SetRenderTarget(0, rtSmall.ptr());
    res = device->SetRenderTarget(0, rtSmall3.ptr());
    res = device->SetRenderTarget(1, rtSmall1.ptr());
    DWORD comp = D3DERR_INVALIDCALL;
    //res = device->SetRenderTarget(1, nullptr);
    //res = device->SetRenderTarget(1, rtMS.ptr());
    //res = device->SetRenderTarget(1, rtSmall1.ptr());
    dxvk::Com<IDirect3DSurface9> test;
    res = device->GetRenderTarget(1, &test);
    res = device->GetViewport(&viewport);
    //res = device->SetDepthStencilSurface(ds.ptr());
    //res = device->SetRenderState(D3DRS_ZENABLE, 0);
    //res = device->SetRenderState(D3DRS_STENCILENABLE, 0);
    //res = device->SetDepthStencilSurface(dsSmall.ptr());
    res = device->SetDepthStencilSurface(nullptr);
    res = device->SetDepthStencilSurface(ds.ptr());
    //res = device->SetDepthStencilSurface(ds.ptr());
    //res = device->SetDepthStencilSurface(nullptr);
    viewport.Width = 640;
    viewport.Height = 480;
    res = device->SetViewport(&viewport);
    res = device->GetViewport(&viewport);
    res = device->GetScissorRect(&rect);
    //res = device->SetRenderTarget(0, rtSmall.ptr());
    res = device->GetViewport(&viewport);
    scissor.right = 999;
    scissor.bottom = 999;
    res = device->SetScissorRect(&scissor);
    res = device->GetScissorRect(&rect);
    //res = device->SetRenderTarget(0, rt.ptr());
    res = device->SetRenderState(D3DRS_ZENABLE, 1);
    res = device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    res = device->SetRenderState(D3DRS_ZWRITEENABLE, 1);
    DWORD writemask = 12345;
    res = device->GetRenderState(D3DRS_COLORWRITEENABLE, &writemask);
    res = device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, verts.data(), 16);
    res = device->GetViewport(&viewport);
    res = device->GetScissorRect(&rect);

    mtrl.Ambient.r = 1.0;
    mtrl.Ambient.g = 0.0;
    mtrl.Ambient.b = 1.0;
    mtrl.Ambient.a = 1.0;
    device->SetMaterial(&mtrl);
    verts = {
        -1.0f, -1.0f, 0.6f, *reinterpret_cast<const float*>(&col),
        -1.0f, 1.0f, 0.6f, *reinterpret_cast<const float*>(&col),
        1.0f, 1.0f, 0.6f, *reinterpret_cast<const float*>(&col),
    };
    res = device->SetRenderTarget(0, rt.ptr());
    //res = device->SetDepthStencilSurface(nullptr);
    //res = device->SetDepthStencilSurface(dsLarge.ptr());
    res = device->SetDepthStencilSurface(ds.ptr());
    //res = device->SetDepthStencilSurface(dsMS.ptr());
    res = device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
    //res = device->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
    res = device->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
    res = device->SetRenderState(D3DRS_ZENABLE, 1);
    //res = device->SetRenderState(D3DRS_ZENABLE, 0);
    res = device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, verts.data(), 16);

    res = rt->GetDesc(&surfDesc);
    RECT srcRect = { 0, 0, (long) surfDesc.Width, (long)surfDesc.Height };
    res = backbuffer->GetDesc(&surfDesc);
    RECT dstRect = { 0, 0, (long)surfDesc.Width, (long)surfDesc.Height };
    res = device->StretchRect(rt.ptr(), &srcRect, backbuffer.ptr(), &dstRect, D3DTEXF_NONE);


    res = ds->GetDesc(&surfDesc);

    res = rtSmall->GetDesc(&surfDesc);
    RECT srcRect1 = { 0, 0, surfDesc.Width, surfDesc.Height };
    res = backbuffer->GetDesc(&surfDesc);
    //RECT dstRect = { 0, 0, surfDesc.Width, surfDesc.Height };
    RECT dstRect1 = { 0, 0, 64, 64 };
    res = device->StretchRect(rtSmall.ptr(), &srcRect1, backbuffer.ptr(), &dstRect1, D3DTEXF_NONE);

    res = device->EndScene();
    res = device->Present(nullptr, nullptr, nullptr, nullptr);
}
