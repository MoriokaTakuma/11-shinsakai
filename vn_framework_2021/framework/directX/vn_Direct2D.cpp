//--------------------------------------------------------------//
//	"vn_Direct2D.cpp"											//
//		Direct2D管理											//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../vn_environment.h"


ID2D1Factory		*vnDirect2D::pD2d1Factory = NULL;
IDWriteFactory		*vnDirect2D::pDWFactory = NULL;
ID2D1RenderTarget	*vnDirect2D::pRenderTarget = NULL;

bool vnDirect2D::initialize(void *hdl, int width, int height)
{
	HWND hWnd = (HWND)hdl;

	HRESULT hr;

	//ID2D1Factoryの生成
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2d1Factory);
	if (FAILED(hr))
	{
		terminate();
		return false;
	}

	//IDWriteFactoryの生成
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWFactory));
	if (FAILED(hr))
	{
		terminate();
		return false;
	}

	//ID2D1HwndRenderTargetの生成
	D2D1_SIZE_U renderTargetSize = { (UINT32)width, (UINT32)height };
	D2D1_PIXEL_FORMAT pixelFormat;
	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRenderTargetProperties;

	pixelFormat.format = DXGI_FORMAT_UNKNOWN;
	pixelFormat.alphaMode = D2D1_ALPHA_MODE_UNKNOWN;

	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	renderTargetProperties.pixelFormat = pixelFormat;
	renderTargetProperties.dpiX = 0;
	renderTargetProperties.dpiY = 0;
	renderTargetProperties.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	renderTargetProperties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	hwndRenderTargetProperties.hwnd = hWnd;
	hwndRenderTargetProperties.pixelSize = renderTargetSize;
	hwndRenderTargetProperties.presentOptions = D2D1_PRESENT_OPTIONS_RETAIN_CONTENTS;

 
	IDXGISurface* pBackBuffer = NULL;
	//ID2D1HwndRenderTargetの生成
	hr = vnDirect3D::getSwapChain()->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (FAILED(hr))
	{
		terminate();
		return false;
	}

	//FLOAT dpiX = 96.0f;
	//FLOAT dpiY = 96.0f;
	//pD2d1Factory->GetDesktopDpi(&dpiX, &dpiY);
	UINT dpi = GetDpiForWindow(hWnd);
	FLOAT dpiX = (FLOAT)dpi;
	FLOAT dpiY = (FLOAT)dpi;

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

	hr = pD2d1Factory->CreateDxgiSurfaceRenderTarget(pBackBuffer, &props, &pRenderTarget);
	if (FAILED(hr))
	{
		terminate();
		return false;
	}

	return true;
}

void vnDirect2D::terminate(void)
{
	SAFE_RELEASE(pRenderTarget);
	SAFE_RELEASE(pDWFactory);
	SAFE_RELEASE(pD2d1Factory);
}

void vnDirect2D::render()
{
	//描画開始(Direct2D)
	pRenderTarget->BeginDraw();

	//フォントの描画
	vnFont::renderString();

	//描画終了(Direct2D)
	pRenderTarget->EndDraw();
}

IDWriteFactory *vnDirect2D::getDWFactory()
{
	return 	pDWFactory;
}

ID2D1RenderTarget *vnDirect2D::getRenderTarget()
{
	return pRenderTarget;
}
