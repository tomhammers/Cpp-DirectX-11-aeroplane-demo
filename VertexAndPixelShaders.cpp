#include "VertexAndPixelShaders.h"

VertexAndPixelShaders::VertexAndPixelShaders()
{
}

VertexAndPixelShaders::~VertexAndPixelShaders()
{
	SAFE_RELEASE(g_pVertexLayout11);
	SAFE_RELEASE(g_pVertexShader);
	SAFE_RELEASE(g_pPixelShader);
	SAFE_RELEASE(pVertexShaderBuffer);
	SAFE_RELEASE(pPixelShaderBuffer);
	SAFE_RELEASE(g_pSkyVertexLayout11)
	SAFE_RELEASE(g_pSkyPixelShader);
	SAFE_RELEASE(pSkyPixelShaderBuffer);
}

void VertexAndPixelShaders::CreateVertexShader(ID3D11Device *device) {
	HRESULT hr = S_OK;
	pVertexShaderBuffer = NULL;
	V(CompileShaderFromFile(L"Tutorial 09 - Meshes Using DXUT Helper Classes_VS.hlsl", "VS_DXUTSDKMesh", "vs_5_0", &pVertexShaderBuffer));

	V(device->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(),
		pVertexShaderBuffer->GetBufferSize(), NULL, &g_pVertexShader));
	DXUT_SetDebugName(g_pVertexShader, "VS_DXUTSDKMesh");
}


void VertexAndPixelShaders::CreatePixelShader(ID3D11Device *device) {
	HRESULT hr = S_OK;
	pPixelShaderBuffer = NULL;
	V(CompileShaderFromFile(L"Tutorial 09 - Meshes Using DXUT Helper Classes_PS.hlsl", "PS_DXUTSDKMesh", "ps_5_0", &pPixelShaderBuffer));

	V(device->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(),
		pPixelShaderBuffer->GetBufferSize(), NULL, &g_pPixelShader));
	DXUT_SetDebugName(g_pPixelShader, "PS_DXUTSDKMesh");	
}


void VertexAndPixelShaders::CreateSkyPixelShader(ID3D11Device *device)
{
	HRESULT hr = S_OK;
	V(CompileShaderFromFile(L"Tutorial 09 - Meshes Using DXUT Helper Classes_PS.hlsl", "PS_DXUTSDKMeshSky", "ps_5_0", &pPixelShaderBuffer));

	V(device->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(),
		pPixelShaderBuffer->GetBufferSize(), NULL, &g_pSkyPixelShader));
	DXUT_SetDebugName(g_pSkyPixelShader, "PS_DXUTSDKMeshSky");

}


// ID3D11DeviceContext *context has inbuild method called CreateInputLayout, so I chose a different name
void VertexAndPixelShaders::MakeInputLayout(ID3D11Device *device) {
	HRESULT hr = S_OK;
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	V(device->CreateInputLayout(layout, ARRAYSIZE(layout), pVertexShaderBuffer->GetBufferPointer(),
		pVertexShaderBuffer->GetBufferSize(), &g_pVertexLayout11));
	DXUT_SetDebugName(g_pVertexLayout11, "Primary");

	SAFE_RELEASE(pVertexShaderBuffer);
	SAFE_RELEASE(pPixelShaderBuffer);

}



void VertexAndPixelShaders::charStrToWideChar(WCHAR *dest, char *source)
{
	int length = strlen(source);
	for (int i = 0; i <= length; i++)
		dest[i] = (WCHAR)source[i];
}





//**************************************************************************//
// Compile the shader file.  These files aren't pre-compiled (well, not		//
// here, and are compiled on he fly).										//
//**************************************************************************//
HRESULT VertexAndPixelShaders::CompileShaderFromFile(WCHAR* szFileName,		// File Name
	LPCSTR szEntryPoint,		// Name of shader
	LPCSTR szShaderModel,		// Shader model
	ID3DBlob** ppBlobOut)	// Blob returned
{
	HRESULT hr = S_OK;
	// find the file
	WCHAR str[MAX_PATH];
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, szFileName));

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(str, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		WCHAR errorCharsW[200];
		if (pErrorBlob != NULL)
		{
			charStrToWideChar(errorCharsW, (char *)pErrorBlob->GetBufferPointer());
			MessageBox(0, errorCharsW, L"Error", 0);
		}
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	SAFE_RELEASE(pErrorBlob);
	return S_OK;
}