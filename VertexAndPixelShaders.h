#pragma once
#include "DXUT.h"
#include "SDKmisc.h"

class VertexAndPixelShaders
{
public:
	ID3D11VertexShader         *g_pVertexShader = NULL;
	ID3D11PixelShader          *g_pPixelShader = NULL;
	ID3DBlob				   *pVertexShaderBuffer = NULL;
	ID3DBlob				   *pPixelShaderBuffer = NULL;
	ID3D11InputLayout          *g_pVertexLayout11 = NULL;
	ID3D11InputLayout          *g_pSkyVertexLayout11 = NULL;

	// sky specific stuff
	ID3D11PixelShader          *g_pSkyPixelShader = NULL;
	ID3DBlob				   *pSkyPixelShaderBuffer = NULL;

public:
	VertexAndPixelShaders();
	~VertexAndPixelShaders();

	void CreateVertexShader(ID3D11Device *device);

	void CreatePixelShader(ID3D11Device *device);

	void CreateSkyPixelShader(ID3D11Device *device);

	void MakeInputLayout(ID3D11Device *device);

	void charStrToWideChar(WCHAR *dest, char *source);

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
};
