#pragma once
#include "DXUT.h"
#include "SDKMesh.h"
#include "Thing.h"



class MeshLoader : public Thing
{
public:
	CDXUTSDKMesh mesh;

	MeshLoader(ID3D11Device *device, ID3D11DeviceContext *context, 
		LPCTSTR meshLocation, int textureNumber, XMMATRIX &matProjection);
	~MeshLoader();


	HRESULT loadMesh(ID3D11Device *device,
		ID3D11DeviceContext *context, LPCTSTR meshLocation, int textureNumber, XMMATRIX &matProjection);

	void renderMesh(ID3D11DeviceContext *context, XMMATRIX &worldViewProjection,
		XMMATRIX &matThingWorld, ID3D11Buffer *pcbVSPerObject, VertexAndPixelShaders *shaders);
};