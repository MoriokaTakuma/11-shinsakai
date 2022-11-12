//--------------------------------------------------------------//
//	"vn_Direct3D.cpp"											//
//		Direct3D管理											//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../vn_environment.h"

#define USE_D3D11_MAP	(0)	//DeviceContext::Map機能が使えない場合などでは0にする

ID3D11Device			*vnDirect3D::pDevice = NULL;
ID3D11DeviceContext		*vnDirect3D::pDeviceContext = NULL;
IDXGIDevice				*vnDirect3D::pDXGI = NULL;
IDXGIAdapter			*vnDirect3D::pAdapter = NULL;
IDXGIFactory			*vnDirect3D::pDXGIFactory = NULL;
IDXGISwapChain			*vnDirect3D::pDXGISwpChain = NULL;
ID3D11RenderTargetView	*vnDirect3D::pRenderTargetView = NULL;
ID3D11DepthStencilView	*vnDirect3D::pDepthStencilView = NULL;

ID3D11Texture2D				*vnDirect3D::pWhiteTexture = NULL;
ID3D11ShaderResourceView	*vnDirect3D::pWhiteTextureView = NULL;


int vnDirect3D::initialize(void *hdl, int width, int height)
{
	HWND hWnd = (HWND)hdl;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;//DirectX11上でDirect2Dを使用するために必要

	//デバイスの生成
	HRESULT hr;
	hr = D3D11CreateDevice(NULL,	//IDXGIAdapter *pAdapter : デバイスの作成時に使用するビデオ アダプターへのポインター(既定のアダプターを使用するにはNULL)
		D3D_DRIVER_TYPE_HARDWARE,	//D3D_DRIVER_TYPE DriverType : 作成するデバイスの種類を表す D3D_DRIVER_TYPE
		NULL,						//HMODULE Software : ソフトウェア ラスタライザーを実装する DLL のハンドル
		createDeviceFlags,			//UINT Flags : 有効にするランタイム レイヤー
		NULL,						//CONST D3D_FEATURE_LEVEL *pFeatureLevels : 作成を試みる機能レベルの順序を指定する D3D_FEATURE_LEVEL の配列へのポインター
		0,							//UINT FeatureLevels : pFeatureLevels の要素数
		D3D11_SDK_VERSION,			//UINT SDKVersion : SDK のバージョン
		&pDevice,					//ID3D11Device **ppDevice : 作成されたデバイスを表す ID3D11Device オブジェクトへのポインターのアドレスを返します。
		NULL,						//D3D_FEATURE_LEVEL *pFeatureLevel : 成功した場合は、成功した pFeatureLevels 配列の最初の D3D_FEATURE_LEVEL を返します。失敗した場合は 0 を返します。
		&pDeviceContext				//ID3D11DeviceContext **ppImmediateContext : デバイス コンテキストを表す ID3D11DeviceContext オブジェクトへのポインターのアドレスを返します。
		);
	if (FAILED(hr))
	{
		terminate();
		return 0;
	}

	//インターフェイス取得
	if (FAILED(pDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGI)))
	{
		terminate();
		return 0;
	}

	//アダプター取得
	if (FAILED(pDXGI->GetAdapter(&pAdapter)))
	{
		terminate();
		return 0;
	}

	//ファクトリー取得
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);
	if (pDXGIFactory == NULL)
	{
		terminate();
		return 0;
	}

	//スワップチェイン作成
	DXGI_SWAP_CHAIN_DESC hDXGISwapChainDesc;
	hDXGISwapChainDesc.BufferDesc.Width = width;
	hDXGISwapChainDesc.BufferDesc.Height = height;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	hDXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	hDXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	hDXGISwapChainDesc.SampleDesc.Count = 1;
	hDXGISwapChainDesc.SampleDesc.Quality = 0;
	hDXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	hDXGISwapChainDesc.BufferCount = 1;
	hDXGISwapChainDesc.OutputWindow = (HWND)hdl;
	hDXGISwapChainDesc.Windowed = TRUE;
	hDXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	hDXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (FAILED(pDXGIFactory->CreateSwapChain(pDevice, &hDXGISwapChainDesc, &pDXGISwpChain)))
	{
		terminate();
		return 0;
	}

	//レンダーターゲットビュー作成
	ID3D11Texture2D* pBackBuffer;

	hr = pDXGISwpChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		terminate();
		return 0;
	}

	hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		terminate();
		return 0;
	}

	//深度・ステンシルバッファ作成
	ID3D11Texture2D* pDepthStencilTexture;
	D3D11_TEXTURE2D_DESC txDesc;
	ZeroMemory(&txDesc, sizeof(txDesc));
	txDesc.Width = width;
	txDesc.Height = height;
	txDesc.MipLevels = 1;
	txDesc.ArraySize = 1;
	txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	txDesc.SampleDesc.Count = 1;
	txDesc.SampleDesc.Quality = 0;
	txDesc.Usage = D3D11_USAGE_DEFAULT;
	txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	txDesc.CPUAccessFlags = 0;
	txDesc.MiscFlags = 0;
	hr = pDevice->CreateTexture2D(&txDesc, NULL, &pDepthStencilTexture);
	if (FAILED(hr))
	{
		terminate();
		return 0;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.Format = txDesc.Format;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateDepthStencilView(pDepthStencilTexture, &dsDesc, &pDepthStencilView);
	if (FAILED(hr))
	{
		terminate();
		return 0;
	}

	//無地(白)のテクスチャを作成
	ZeroMemory(&txDesc, sizeof(txDesc));
	txDesc.Width = 16;
	txDesc.Height = 16;
	txDesc.MipLevels = 1;
	txDesc.ArraySize = 1;
	txDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	txDesc.SampleDesc.Count = 1;
	txDesc.SampleDesc.Quality = 0;
	txDesc.Usage = D3D11_USAGE_DYNAMIC;
	txDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	txDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	txDesc.MiscFlags = 0;

#if USE_D3D11_MAP
	hr = pDevice->CreateTexture2D(&txDesc, NULL, &pWhiteTexture);

	D3D11_MAPPED_SUBRESOURCE msr;
	hr = pDeviceContext->Map(pWhiteTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	BYTE* p = (BYTE*)msr.pData;
	for (int i = 0; i < (int)msr.DepthPitch; i++)
	{
		p[i] = 0xff;
	}
	pDeviceContext->Unmap(pWhiteTexture, 0);
#else
	D3D11_SUBRESOURCE_DATA initialData;
	ZeroMemory(&initialData, sizeof(D3D11_SUBRESOURCE_DATA));
	initialData.pSysMem = new BYTE[16 * 16 * 4];
	initialData.SysMemPitch = 16 * 4;
	memset((void*)initialData.pSysMem, 0xff, 16 * 16 * 4);

	hr = pDevice->CreateTexture2D(&txDesc, &initialData, &pWhiteTexture);

	delete[] initialData.pSysMem;
#endif

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = pDevice->CreateShaderResourceView(pWhiteTexture, &srvDesc, &pWhiteTextureView);

	return 1;
}

void vnDirect3D::terminate(void)
{
	SAFE_RELEASE(pWhiteTextureView);
	SAFE_RELEASE(pWhiteTexture);

	SAFE_RELEASE(pDepthStencilView);
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pDXGISwpChain);
	SAFE_RELEASE(pDXGIFactory);
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pDXGI);
	SAFE_RELEASE(pDeviceContext);
	SAFE_RELEASE(pDevice);
}

ID3D11Device* vnDirect3D::getDevice(void)
{
	return pDevice;
}

ID3D11DeviceContext* vnDirect3D::getDeviceContext(void)
{
	return pDeviceContext;
}

IDXGISwapChain* vnDirect3D::getSwapChain(void)
{
	return pDXGISwpChain;
}

void vnDirect3D::draw(UINT vertexCount, UINT startVertexLocation)
{
	pDeviceContext->Draw(vertexCount, startVertexLocation);
}

void vnDirect3D::drawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	pDeviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void vnDirect3D::clear(void)
{
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, vnMainFrame::clearColor);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void vnDirect3D::present(UINT SyncInterval)
{
	pDXGISwpChain->Present(SyncInterval, 0);
}

void vnDirect3D::setVShader(vnShader::eVertexShader vs)
{
	//頂点シェーダーをコンテキストに設定
	pDeviceContext->VSSetShader(vnShader::getVShader(vs)->getShader(), NULL, 0);
}

void vnDirect3D::setPShader(vnShader::ePixelShader ps)
{
	//ピクセルシェーダーをコンテキストに設定
	pDeviceContext->PSSetShader(vnShader::getPShader(ps)->getShader(), NULL, 0);
}

void vnDirect3D::setShaderResource(ID3D11ShaderResourceView* p, int slot)
{
	ID3D11ShaderResourceView* hpShaderResourceViews[] = { p ? p : pWhiteTextureView };
	vnDirect3D::getDeviceContext()->PSSetShaderResources(slot, 1, hpShaderResourceViews);
}
