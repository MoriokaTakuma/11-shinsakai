//--------------------------------------------------------------//
//	"vn_Direct2D.h"												//
//		Direct2Dä«óù											//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnDirect2D
{
private:
	static ID2D1Factory			*pD2d1Factory;
	static IDWriteFactory		*pDWFactory;
	static ID2D1RenderTarget	*pRenderTarget;

public:
	static bool initialize(void *hdl, int width, int height);
	static void terminate(void);

	static void render();

	static IDWriteFactory		*getDWFactory();
	static ID2D1RenderTarget	*getRenderTarget();

	
};

