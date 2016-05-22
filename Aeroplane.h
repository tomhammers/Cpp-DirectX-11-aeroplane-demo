#pragma once
#include "ObjLoader.h"

class Aeroplane
{
private:
	
	D3D_DRIVER_TYPE				g_driverType = D3D_DRIVER_TYPE_NULL;

public:

	ObjLoader *plane;
	ObjLoader *propellor;

	Aeroplane();
	~Aeroplane();

	void buildPlane(ID3D11Device *device, ID3D11DeviceContext *context, XMMATRIX &matProjection);

	void renderPlane(ID3D11DeviceContext *context,
		XMMATRIX &view, XMMATRIX &matProjection, ID3D11Buffer *pcbVSPerObject, VertexAndPixelShaders *shaders);

	float spinPropellor();

	void throttle(float frameTime);
};

