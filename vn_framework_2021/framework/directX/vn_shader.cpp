//--------------------------------------------------------------//
//	"vn_shader.cpp"												//
//		シェーダ管理											//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../vn_environment.h"

vnShader::vnVertexShader **vnShader::vshader = NULL;
vnShader::vnPixelShader **vnShader::pshader = NULL;

bool vnShader::initialize()
{
	//頂点シェーダの作成
	vshader = new vnVertexShader * [(unsigned int)eVertexShader::VS_MAX];
	if (vshader)
	{
		vshader[(unsigned int)eVertexShader::VS_2D] = new vnVertexShader("shader/vs_2d.cso");
	}

	//ピクセルシェーダの作成
	pshader = new vnPixelShader * [(unsigned int)ePixelShader::PS_MAX];
	if (pshader)
	{
		pshader[(unsigned int)ePixelShader::PS_2D] = new vnPixelShader("shader/ps_2d.cso");
	}
	return (vshader!=NULL && pshader!=NULL);
}

void vnShader::terminate()
{
	for (int i = 0; i < (unsigned int)eVertexShader::VS_MAX; i++)
	{
		delete vshader[i];
		vshader[i] = NULL;
	}
	delete[] vshader;
	vshader = NULL;

	for (int i = 0; i < (unsigned int)ePixelShader::PS_MAX; i++)
	{
		delete pshader[i];
		pshader[i] = NULL;
	}
	delete[] pshader;
	pshader = NULL;
}

vnShader::vnShaderBase::vnShaderBase(const char *path)
{
	code = NULL;
	length = 0;

	FILE *fp = NULL;
	if ((fopen_s(&fp, path, "rb")) != 0)
	{
		assert(false);
		return;
	}
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	code = new BYTE[length];
	fread((void*)code, length, 1, fp);

	fclose(fp);
}

vnShader::vnShaderBase::~vnShaderBase()
{
	if(code)delete[] code;
}

vnShader::vnVertexShader::vnVertexShader(const char *path) : vnShaderBase(path)
{
	vs = NULL;
	if (!code)return;

	HRESULT hr;
	hr = vnDirect3D::getDevice()->CreateVertexShader(
		code,
		length,
		NULL,
		&vs
		);
	if (hr != S_OK)
	{
		assert(false);
	}
}

vnShader::vnVertexShader::~vnVertexShader()
{
	if (vs)vs->Release();
}


vnShader::vnPixelShader::vnPixelShader(const char *path) : vnShaderBase(path)
{
	ps = NULL;
	if (!code)return;

	HRESULT hr;
	hr = vnDirect3D::getDevice()->CreatePixelShader(
		code,
		length,
		NULL,
		&ps
		);
	if (hr != S_OK)
	{
		assert(false);
	}
}

vnShader::vnPixelShader::~vnPixelShader()
{
	if (ps)ps->Release();
}


