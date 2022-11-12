//--------------------------------------------------------------//
//	"vn_Direct3D.h"												//
//		Direct3D管理											//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnDirect3D
{
private:
	//DirectXインターフェイス
	static ID3D11Device				*pDevice;			//Direct3D11デバイス
	static ID3D11DeviceContext		*pDeviceContext;	//Direct3D11デバイスコンテキスト
	static IDXGIDevice				*pDXGI;				//DXGIデバイス
	static IDXGIAdapter				*pAdapter;			//アダプタ
	static IDXGIFactory				*pDXGIFactory;		//ファクトリ
	static IDXGISwapChain			*pDXGISwpChain;		//スワップチェイン
	static ID3D11RenderTargetView	*pRenderTargetView;	//レンダーターゲットビュー
	static ID3D11DepthStencilView	*pDepthStencilView;	//デプスステンシルビュー

	//テクスチャ
	static ID3D11Texture2D			*pWhiteTexture;
	static ID3D11ShaderResourceView	*pWhiteTextureView;

public:
	//フレームワーク管理
	static int initialize(void *hdl, int width, int height);
	static void terminate(void);

	//インターフェイスの取得
	static ID3D11Device* getDevice(void);
	static ID3D11DeviceContext* getDeviceContext(void);
	static IDXGISwapChain* getSwapChain(void);

	//描画コール
	static void draw(UINT vertexCount, UINT startVertexLocation = 0);
	static void drawIndexed(UINT indexCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0);

	//画面管理
	static void clear(void);
	static void present(UINT SyncInterval = 0);

	//シェーダーのセット
	static void setVShader(vnShader::eVertexShader vs);
	static void setPShader(vnShader::ePixelShader ps);

	//シェーダーリソースのセット
	static void setShaderResource(ID3D11ShaderResourceView* p, int slot = 0);
};
