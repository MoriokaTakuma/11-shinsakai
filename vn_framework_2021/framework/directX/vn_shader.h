//--------------------------------------------------------------//
//	"vn_shader.h"												//
//		シェーダ管理											//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnShader
{
public:
	enum class eVertexShader
	{
		VS_2D,
		VS_MAX,
	};

	enum class ePixelShader
	{
		PS_2D,
		PS_MAX,
	};

	class vnShaderBase
	{
	protected:
		const BYTE *code;
		long length;
	public:
		vnShaderBase(const char *path);
		~vnShaderBase();

		const BYTE *getCode(){ return code; }
		long getLength(){ return length; }
	};

	class vnVertexShader : public vnShaderBase
	{
	private:
		ID3D11VertexShader *vs;

	public:
		vnVertexShader(const char *path);
		~vnVertexShader();

		ID3D11VertexShader *getShader(){ return vs; }
	};

	class vnPixelShader : public vnShaderBase
	{
	private:
		ID3D11PixelShader *ps;

	public:
		vnPixelShader(const char *path);
		~vnPixelShader();

		ID3D11PixelShader *getShader(){ return ps; }
	};

private:
	static vnVertexShader **vshader;
	static vnPixelShader **pshader;

public:
	static bool initialize();
	static void terminate();

	static vnVertexShader* getVShader(eVertexShader vs) { return vshader[(unsigned int)vs]; }
	static vnPixelShader* getPShader(ePixelShader ps) { return pshader[(unsigned int)ps]; }
};