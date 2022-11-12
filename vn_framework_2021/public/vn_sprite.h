//--------------------------------------------------------------//
//	"vn_sprite.h"												//
//		スプライトクラス										//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//ポリゴン頂点構造体
struct vnVertex2D
{
	float x, y;			//座標
	float r, g, b, a;	//カラー
	float u, v;			//uv
};

//スプライトクラス
class vnSprite
{
public:
	//座標
	float posX;
	float posY;
	//サイズ
	float sizeX;
	float sizeY;
	//スケール
	float scaleX;
	float scaleY;
	//回転
	float rot;

	//頂点数
	static const int VertexNum = 4;
	//頂点データ
	vnVertex2D	vtx[4];

private:
	//描画優先順位(数値が大きいほど手前に描画される)
	int renderPriority;

	//実行状態の管理(execute関数を実行する)
	bool executeEnable;

	//描画状態の管理(render関数を実行する)
	bool renderEnable;

protected:
	//頂点データへの各種情報の設定
	virtual void setVertexPosition();

private:
	//頂点バッファ
	ID3D11Buffer				*pVertexBuffer;

	//テクスチャ
	ID3D11Resource				*pTexture;
	ID3D11ShaderResourceView	*pTextureView;

	//静的共通データ
	static ID3D11RasterizerState	*pRasterizerState;
	static ID3D11SamplerState		*pSamplerState;
	static ID3D11BlendState			*pBlendState;
	static ID3D11DepthStencilState	*pDepthStencilState;
	static ID3D11InputLayout		*pInputLayout;
	static ID3D11Buffer				*pConstantBuffer;
	static UINT						VertexStrides;
	static UINT						VertexOffsets;

public:
	//静的共通データ初期化
	static bool initializeCommon();

	//静的共通データ削除
	static void terminateCommon();

	//コンストラクタ
	vnSprite(float x, float y, float width, float height, const WCHAR *texture_file, float left_u = 0.0f, float right_u = 1.0f, float top_v = 0.0f, float bottom_v = 1.0f);

	//デストラクタ
	virtual ~vnSprite();

	//処理
	virtual void execute();

	//描画
	virtual void render();

	//描画優先順位の設定
	void setRenderPriority(int value);

	//描画優先順位の取得
	int getRenderPriority(void);

	//実行状態の設定
	void setExecuteEnable(bool flag);

	//描画状態の設定
	void setRenderEnable(bool flag);

	//実行状態の取得
	bool isExecuteEnable();

	//描画状態の取得
	bool isRenderEnable();
};
