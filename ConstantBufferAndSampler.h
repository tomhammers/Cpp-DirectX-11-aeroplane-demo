#pragma once

#include <d3d11.h>
#include <xnamath.h>
#include "DXUT.h"

class ConstantBufferAndSampler
{
public:
	struct CB_VS_PER_OBJECT
	{
		XMMATRIX matWorldViewProj;
		XMMATRIX matWorld;				// needed to transform the normals.
	};

	//**************************************************************************//
	// These are structures we pass to the pixel shader.  						//
	// Note we do it properly here and pass the WVP matrix, rather than world,	//
	// view and projection matrices separately.									//
	//																			//
	// These structures must be identical to those defined in the shader that	//
	// you use.  So much for encapsulation; Roy	Tucker (Comp Sci students will  //
	// know him) will not approve.												//
	//**************************************************************************//
	struct CB_PS_PER_OBJECT
	{
		XMFLOAT4 m_vObjectColor;
	};
	UINT                        g_iCBPSPerObjectBind = 0;

	struct CB_PS_PER_FRAME
	{
		XMVECTOR m_vLightDirAmbient;	// Vector pointing at the light
	};

	
	//**************************************************************************//
	// Now a global instance of each constant buffer.							//
	//**************************************************************************//
	ID3D11Buffer               *g_pcbVSPerObject = NULL;
	ID3D11Buffer               *g_pcbPSPerObject = NULL;
	ID3D11Buffer               *g_pcbPSPerFrame = NULL;
	ID3D11SamplerState*                 g_pSamplerLinear = NULL;

public:
	ConstantBufferAndSampler();
	~ConstantBufferAndSampler();

	void CreateConstandBuffersAndSampler(ID3D11Device *device);
};

