#include "StretchRectTest.h"

enum class D3D9Format : uint32_t {
    Unknown = 0,

    R8G8B8 = 20,
    A8R8G8B8 = 21,
    X8R8G8B8 = 22,
    R5G6B5 = 23,
    X1R5G5B5 = 24,
    A1R5G5B5 = 25,
    A4R4G4B4 = 26,
    R3G3B2 = 27,
    A8 = 28,
    A8R3G3B2 = 29,
    X4R4G4B4 = 30,
    A2B10G10R10 = 31,
    A8B8G8R8 = 32,
    X8B8G8R8 = 33,
    G16R16 = 34,
    A2R10G10B10 = 35,
    A16B16G16R16 = 36,
    A8P8 = 40,
    P8 = 41,
    L8 = 50,
    A8L8 = 51,
    A4L4 = 52,
    V8U8 = 60,
    L6V5U5 = 61,
    X8L8V8U8 = 62,
    Q8W8V8U8 = 63,
    V16U16 = 64,
    W11V11U10 = 65,
    A2W10V10U10 = 67,
    UYVY = MAKEFOURCC('U', 'Y', 'V', 'Y'),
    R8G8_B8G8 = MAKEFOURCC('R', 'G', 'B', 'G'),
    YUY2 = MAKEFOURCC('Y', 'U', 'Y', '2'),
    G8R8_G8B8 = MAKEFOURCC('G', 'R', 'G', 'B'),
    DXT1 = MAKEFOURCC('D', 'X', 'T', '1'),
    DXT2 = MAKEFOURCC('D', 'X', 'T', '2'),
    DXT3 = MAKEFOURCC('D', 'X', 'T', '3'),
    DXT4 = MAKEFOURCC('D', 'X', 'T', '4'),
    DXT5 = MAKEFOURCC('D', 'X', 'T', '5'),
    D16_LOCKABLE = 70,
    D32 = 71,
    D15S1 = 73,
    D24S8 = 75,
    D24X8 = 77,
    D24X4S4 = 79,
    D16 = 80,
    D32F_LOCKABLE = 82,
    D24FS8 = 83,
    D32_LOCKABLE = 84,
    S8_LOCKABLE = 85,
    L16 = 81,
    VERTEXDATA = 100,
    INDEX16 = 101,
    INDEX32 = 102,
    Q16W16V16U16 = 110,
    MULTI2_ARGB8 = MAKEFOURCC('M', 'E', 'T', '1'),
    R16F = 111,
    G16R16F = 112,
    A16B16G16R16F = 113,
    R32F = 114,
    G32R32F = 115,
    A32B32G32R32F = 116,
    CxV8U8 = 117,
    A1 = 118,
    A2B10G10R10_XR_BIAS = 119,
    BINARYBUFFER = 199,

    // Driver Hacks / Unofficial Formats
    ATI1 = MAKEFOURCC('A', 'T', 'I', '1'),
    ATI2 = MAKEFOURCC('A', 'T', 'I', '2'),
    INST = MAKEFOURCC('I', 'N', 'S', 'T'),
    DF24 = MAKEFOURCC('D', 'F', '2', '4'),
    DF16 = MAKEFOURCC('D', 'F', '1', '6'),
    NULL_FORMAT = MAKEFOURCC('N', 'U', 'L', 'L'),
    GET4 = MAKEFOURCC('G', 'E', 'T', '4'),
    GET1 = MAKEFOURCC('G', 'E', 'T', '1'),
    NVDB = MAKEFOURCC('N', 'V', 'D', 'B'),
    A2M1 = MAKEFOURCC('A', '2', 'M', '1'),
    A2M0 = MAKEFOURCC('A', '2', 'M', '0'),
    ATOC = MAKEFOURCC('A', 'T', 'O', 'C'),
    INTZ = MAKEFOURCC('I', 'N', 'T', 'Z'),
    RAWZ = MAKEFOURCC('R', 'A', 'W', 'Z'),
    RESZ = MAKEFOURCC('R', 'E', 'S', 'Z'),

    NV11 = MAKEFOURCC('N', 'V', '1', '1'),
    NV12 = MAKEFOURCC('N', 'V', '1', '2'),
    P010 = MAKEFOURCC('P', '0', '1', '0'), // Same as NV12 but 10 bit
    P016 = MAKEFOURCC('P', '0', '1', '6'), // Same as NV12 but 16 bit
    Y210 = MAKEFOURCC('Y', '2', '1', '0'),
    Y216 = MAKEFOURCC('Y', '2', '1', '6'),
    Y410 = MAKEFOURCC('Y', '4', '1', '0'),
    AYUV = MAKEFOURCC('A', 'Y', 'U', 'V'),
    YV12 = MAKEFOURCC('Y', 'V', '1', '2'),
    OPAQUE_420 = MAKEFOURCC('4', '2', '0', 'O'),

    // Not supported but exist
    AI44 = MAKEFOURCC('A', 'I', '4', '4'),
    IA44 = MAKEFOURCC('I', 'A', '4', '4'),
    R2VB = MAKEFOURCC('R', '2', 'V', 'B'),
    COPM = MAKEFOURCC('C', 'O', 'P', 'M'),
    SSAA = MAKEFOURCC('S', 'S', 'A', 'A'),
    AL16 = MAKEFOURCC('A', 'L', '1', '6'),
    R16 = MAKEFOURCC(' ', 'R', '1', '6'),

    EXT1 = MAKEFOURCC('E', 'X', 'T', '1'),
    FXT1 = MAKEFOURCC('F', 'X', 'T', '1'),
    GXT1 = MAKEFOURCC('G', 'X', 'T', '1'),
    HXT1 = MAKEFOURCC('H', 'X', 'T', '1'),
};

bool CheckFormatUsages(IDirect3D9* d3d9, D3D9Format Format, D3DRESOURCETYPE RType, DWORD& supportedUsage) {
    supportedUsage = 0;
    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, RType, (D3DFORMAT)Format) != S_OK) {
        return false;
    }
    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, RType, (D3DFORMAT)Format) == S_OK) {
        supportedUsage |= D3DUSAGE_DEPTHSTENCIL;
    }
    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_AUTOGENMIPMAP, RType, (D3DFORMAT)Format) == S_OK) {
        supportedUsage |= D3DUSAGE_AUTOGENMIPMAP;
    }
    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_DYNAMIC, RType, (D3DFORMAT)Format) == S_OK) {
        supportedUsage |= D3DUSAGE_DYNAMIC;
    }
    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_WRITEONLY, RType, (D3DFORMAT)Format) == S_OK) {
        supportedUsage |= D3DUSAGE_WRITEONLY;
    }
    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_SOFTWAREPROCESSING, RType, (D3DFORMAT)Format) == S_OK) {
        supportedUsage |= D3DUSAGE_SOFTWAREPROCESSING;
    }
    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_RENDERTARGET, RType, (D3DFORMAT)Format) == S_OK) {
        supportedUsage |= D3DUSAGE_RENDERTARGET;
    }
    return true;
}

DWORD CheckFormatRTypes(IDirect3D9* d3d9, D3D9Format Format, DWORD Usage) {
    DWORD supportedRtypes = 0;

    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, Usage, D3DRTYPE_SURFACE, (D3DFORMAT)Format) == S_OK) {
        supportedRtypes |= 1 << D3DRTYPE_SURFACE;
    }

    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, Usage, D3DRTYPE_VOLUME, (D3DFORMAT)Format) == S_OK) {
        supportedRtypes |= 1 << D3DRTYPE_VOLUME;
    }

    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, Usage, D3DRTYPE_TEXTURE, (D3DFORMAT)Format) == S_OK) {
        supportedRtypes |= 1 << D3DRTYPE_TEXTURE;
    }

    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, Usage, D3DRTYPE_VOLUMETEXTURE, (D3DFORMAT)Format) == S_OK) {
        supportedRtypes |= 1 << D3DRTYPE_VOLUMETEXTURE;
    }

    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, Usage, D3DRTYPE_CUBETEXTURE, (D3DFORMAT)Format) == S_OK) {
        supportedRtypes |= 1 << D3DRTYPE_CUBETEXTURE;
    }

    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, Usage, D3DRTYPE_VERTEXBUFFER, (D3DFORMAT)Format) == S_OK) {
        supportedRtypes |= 1 << D3DRTYPE_VERTEXBUFFER;
    }

    if (d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, Usage, D3DRTYPE_INDEXBUFFER, (D3DFORMAT)Format) == S_OK) {
        supportedRtypes |= 1 << D3DRTYPE_INDEXBUFFER;
    }

    return supportedRtypes >> 1;
}

StretchRectTest::StretchRectTest(HWND window, dxvk::Com<IDirect3D9>&& d3d9, dxvk::Com<IDirect3DDevice9>&& device)
	: D3D9Test(window, std::move(d3d9), std::move(device))
{
	IDirect3DDevice9Ex* deviceEx = static_cast<IDirect3DDevice9Ex*>(device.ptr());

    HRESULT res;

    D3DCAPS9 caps;
    res = d3d9->GetDeviceCaps(0, D3DDEVTYPE_HAL, &caps);

	res = device->CreateDepthStencilSurface(1280, 720, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, false, &src, nullptr);
	res = device->CreateDepthStencilSurface(1280, 720, D3DFMT_D32F_LOCKABLE, D3DMULTISAMPLE_NONE, 0, true, &dst, nullptr); // Lockable + Discard fails
	res = device->CreateDepthStencilSurface(1280, 720, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, true, &dst, nullptr);

	res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8);
	res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_TEXTURE, D3DFMT_D24X8);
	res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32_LOCKABLE);
	res = d3d9->CheckDepthStencilMatch(0, D3DDEVTYPE_HAL, D3DFMT_D32_LOCKABLE, D3DFMT_X8R8G8B8, D3DFMT_D24X8);
	res = deviceEx->CreateDepthStencilSurfaceEx(1280, 720, D3DFMT_D32F_LOCKABLE, D3DMULTISAMPLE_NONE, 0, false, &src, nullptr, 0);
	//res = deviceEx->CreateDepthStencilSurfaceEx(1280, 720, D3DFMT_D32_LOCKABLE, D3DMULTISAMPLE_NONE, 0, false, &src, nullptr, 0);
	//res = deviceEx->CreateDepthStencilSurfaceEx(1280, 720, D3DFMT_D32_LOCKABLE, D3DMULTISAMPLE_NONE, 0, false, &src, nullptr, D3DUSAGE_DEPTHSTENCIL);

    DWORD supportedUsage;
    bool supported = CheckFormatUsages(d3d9.ptr(), D3D9Format::YUY2, D3DRTYPE_TEXTURE, supportedUsage);
    supported = CheckFormatUsages(d3d9.ptr(), D3D9Format::YUY2, D3DRTYPE_SURFACE, supportedUsage);

    DWORD supportedRTypes = CheckFormatRTypes(d3d9.ptr(), D3D9Format::YUY2, 0);
    supportedRTypes = CheckFormatRTypes(d3d9.ptr(), D3D9Format::UYVY, 0);
    supportedRTypes = CheckFormatRTypes(d3d9.ptr(), D3D9Format::X8R8G8B8, 0);
    supportedRTypes = CheckFormatRTypes(d3d9.ptr(), D3D9Format::X8R8G8B8, D3DUSAGE_RENDERTARGET);
    supportedRTypes = CheckFormatRTypes(d3d9.ptr(), D3D9Format::NV12, 0);

    dxvk::Com<IDirect3DSurface9> src;
    dxvk::Com<IDirect3DTexture9> dst;
    res = device->CreateOffscreenPlainSurface(128, 128, D3DFMT_YUY2, D3DPOOL_DEFAULT, &src, nullptr);
    res = device->CreateTexture(128, 128, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &dst, nullptr);

    dxvk::Com<IDirect3DSurface9> dstSurf;
    dst->GetSurfaceLevel(0, &dstSurf);
    RECT srcRect = { 16, 16, 32, 32 };
    RECT dstRect = { 32, 32, 48, 48};
    res = device->StretchRect(src.ptr(), &srcRect, dstSurf.ptr(), &dstRect, D3DTEXF_LINEAR);
    res = device->StretchRect(src.ptr(), &srcRect, dstSurf.ptr(), &dstRect, D3DTEXF_ANISOTROPIC);
    res = device->StretchRect(src.ptr(), &srcRect, dstSurf.ptr(), &dstRect, D3DTEXF_NONE);
    res = device->StretchRect(dstSurf.ptr(), &srcRect, src.ptr(), &dstRect, D3DTEXF_NONE);
    dstRect.right = 64;
    dstRect.bottom = 64;
    res = device->StretchRect(src.ptr(), &srcRect, dstSurf.ptr(), &dstRect, D3DTEXF_NONE);
    res = device->StretchRect(dstSurf.ptr(), &srcRect, src.ptr(), &dstRect, D3DTEXF_NONE);




    res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, D3DFMT_YUY2);
    dxvk::Com<IDirect3DTexture9> texture;
    dxvk::Com<IDirect3DSurface9> surface;
    res = device->CreateTexture(128, 128, 1, 0, D3DFMT_YUY2, D3DPOOL_DEFAULT, &texture, nullptr);
    res = device->CreateOffscreenPlainSurface(128, 128, D3DFMT_YUY2, D3DPOOL_DEFAULT, &surface, nullptr);
    res = device->CreateTexture(128, 128, 1, 0, D3DFMT_YUY2, D3DPOOL_SYSTEMMEM, &texture, nullptr);
    res = device->CreateOffscreenPlainSurface(128, 128, D3DFMT_YUY2, D3DPOOL_SYSTEMMEM, &surface, nullptr);
    res = device->CreateTexture(128, 128, 1, 0, D3DFMT_YUY2, D3DPOOL_SCRATCH, &texture, nullptr);
    res = device->CreateOffscreenPlainSurface(128, 128, D3DFMT_YUY2, D3DPOOL_SCRATCH, &surface, nullptr);
    res = device->CreateTexture(128, 128, 1, 0, D3DFMT_YUY2, D3DPOOL_MANAGED, &texture, nullptr);
    res = device->CreateOffscreenPlainSurface(128, 128, D3DFMT_YUY2, D3DPOOL_MANAGED, &surface, nullptr);
    res = device->CreateOffscreenPlainSurface(128, 128, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &surface, nullptr);


	res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, D3DFMT_X8R8G8B8);
	res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, D3DFMT_YUY2);
	res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, D3DFMT_UYVY);
    res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, (D3DFORMAT)D3D9Format::NV12);
    res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, (D3DFORMAT)D3D9Format::NV11);
    res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, (D3DFORMAT)D3D9Format::W11V11U10);
	res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, D3DFMT_X8L8V8U8);
	res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, D3DFMT_A2W10V10U10);
	res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, D3DFMT_Q16W16V16U16);
	res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_TEXTURE, D3DFMT_D24X8);
	res = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32_LOCKABLE);

	res = device->CreateTexture(16, 16, 3, 0, D3DFMT_DXT5, D3DPOOL_MANAGED, &texture, nullptr);
	res = texture->GetSurfaceLevel(0, &surface);


	res = device->CreateTexture(1280, 720, 3, 0, D3DFMT_D24X8, D3DPOOL_MANAGED, &texture, nullptr);
	res = device->CreateTexture(1280, 720, 3, 0, D3DFMT_D24X8, D3DPOOL_DEFAULT, &texture, nullptr);
	res = device->SetTexture(0, texture.ptr());

	D3DLOCKED_RECT locked;
	res = surface->LockRect(&locked, nullptr, 0);
	res = device->Present(nullptr, nullptr, nullptr, nullptr);
	res = surface->LockRect(&locked, nullptr, 0);
	res = surface->LockRect(&locked, nullptr, 0);
	res = surface->LockRect(&locked, nullptr, 0);
	res = surface->LockRect(&locked, nullptr, 0);
	res = surface->UnlockRect();

	dxvk::Com<IDirect3DSurface9> backbuffer;
	res = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	res = backbuffer->LockRect(&locked, nullptr, 0);
	res = backbuffer->UnlockRect();
}

void StretchRectTest::Render()
{
	HRESULT res;
	RECT rect = { 0, 0, 1280, 720 };
	// Discard on either src or dst makes stretch rect fail
	res = device->StretchRect(src.ptr(), &rect, dst.ptr(), &rect, D3DTEXF_NONE);

	res = device->Present(nullptr, nullptr, nullptr, nullptr);
}
