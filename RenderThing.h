#pragma once
#include "DXUT.h"
#include "SDKMesh.h"
#include "Thing.h"

class RenderThing : public Thing
{
public:
	CDXUTSDKMesh mesh;
	ID3D11Device *g_pd3dDevice;
	ID3D11InputLayout *g_pVertexLayout11;

	RenderThing(ID3D11Device        *pRenderingDevice,
		ID3D11DeviceContext *pImmediateContext, LPCTSTR meshLocation);
};