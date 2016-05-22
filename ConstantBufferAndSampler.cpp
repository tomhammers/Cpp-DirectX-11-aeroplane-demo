#include "ConstantBufferAndSampler.h"


ConstantBufferAndSampler::ConstantBufferAndSampler()
{
}


ConstantBufferAndSampler::~ConstantBufferAndSampler()
{
	SAFE_RELEASE( g_pcbVSPerObject );
	SAFE_RELEASE( g_pcbPSPerObject );
	SAFE_RELEASE( g_pcbPSPerFrame );
	SAFE_RELEASE( g_pSamplerLinear );
}

void ConstantBufferAndSampler::CreateConstandBuffersAndSampler(ID3D11Device *device)
{
	HRESULT hr = 0;

	D3D11_SAMPLER_DESC SamDesc;
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.MipLODBias = 0.0f;
	SamDesc.MaxAnisotropy = 1;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamDesc.BorderColor[0] = SamDesc.BorderColor[1] = SamDesc.BorderColor[2] = SamDesc.BorderColor[3] = 0;
	SamDesc.MinLOD = 0;
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
	V(device->CreateSamplerState(&SamDesc, &g_pSamplerLinear));
	DXUT_SetDebugName(g_pSamplerLinear, "Primary");

	////**************************************************************************//
	//// Create the 3 constant bufers, using the same buffer descriptor to create //
	//// all three.																//
	////**************************************************************************//
	D3D11_BUFFER_DESC Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = 0;
	Desc.MiscFlags = 0;

	Desc.ByteWidth = sizeof(CB_VS_PER_OBJECT);
	V(device->CreateBuffer(&Desc, NULL, &g_pcbVSPerObject));
	DXUT_SetDebugName(g_pcbVSPerObject, "CB_VS_PER_OBJECT");

	Desc.ByteWidth = sizeof(CB_PS_PER_OBJECT);
	V(device->CreateBuffer(&Desc, NULL, &g_pcbPSPerObject));
	DXUT_SetDebugName(g_pcbPSPerObject, "CB_PS_PER_OBJECT");

	Desc.ByteWidth = sizeof(CB_PS_PER_FRAME);
	V(device->CreateBuffer(&Desc, NULL, &g_pcbPSPerFrame));
	DXUT_SetDebugName(g_pcbPSPerFrame, "CB_PS_PER_FRAME");
}