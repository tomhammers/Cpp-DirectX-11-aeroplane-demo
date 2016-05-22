#include "Aeroplane.h"


Aeroplane::Aeroplane()
{
	plane = new ObjLoader();
	propellor = new ObjLoader();
}


Aeroplane::~Aeroplane()
{
}

void Aeroplane::buildPlane(ID3D11Device *device, ID3D11DeviceContext *context, XMMATRIX &matProjection)
{
	plane->loadObj("Media\\Airplane\\plane6.obj", device, context, matProjection, L"Media\\airplane\\plane.jpg");
	propellor->loadObj("Media\\Airplane\\propeller3.obj", device, context, matProjection, L"");
}

void Aeroplane::renderPlane(ID3D11DeviceContext *context, 
	XMMATRIX &view, XMMATRIX &matProjection, ID3D11Buffer *pcbVSPerObject, VertexAndPixelShaders *shaders)
{
	// Plane
	XMMATRIX matPlaneTranslate = XMMatrixTranslation(plane->getPositionX(), plane->getPositionY(), plane->getPositionZ());
	XMMATRIX matPlaneRotateY = XMMatrixRotationY(3.14159); // 180 degrees
	XMMATRIX matPlaneRotate = XMMatrixRotationRollPitchYaw(plane->getRotationX(), plane->getRotationY(), plane->getRotationZ());
	XMMATRIX matPlaneScale = XMMatrixScaling(1.0, 1.0, 1.0);
	XMMATRIX matPlaneWorld = matPlaneRotateY * matPlaneRotate * matPlaneTranslate * matPlaneScale;
	XMMATRIX matPlaneWorldViewProjection = matPlaneWorld * view * matProjection;


	// Propeller
	XMMATRIX matPropTranslate = XMMatrixTranslation(0.0, 0.0, 0.0);
	XMMATRIX matPropRotateX = XMMatrixRotationZ(spinPropellor()); 
	XMMATRIX matPropScale = XMMatrixScaling(1.0, 1.0, 1.0);
	XMMATRIX matPropWorld = matPropRotateX * matPropTranslate * matPropScale;
	matPropWorld *= matPlaneWorld; // using matrix magic to get the propeller to move relative to the plane
	XMMATRIX matPropWorldViewProjection = matPropWorld * view * matProjection;

	plane->renderMesh(context, matPlaneWorldViewProjection, matPlaneWorld, matProjection, pcbVSPerObject, shaders);
	propellor->renderMesh(context, matPropWorldViewProjection, matPropWorld, matProjection, pcbVSPerObject, shaders);
}


float Aeroplane::spinPropellor()
{
	// shamelessly taken from Start of Obj Loader tutorial to help rotate the propeller
	static float t = 0.0f;
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;
		t = (dwTimeCur - dwTimeStart) / 2000.0f; // was 1000.0f
	}

	return -t * 40;
}

void Aeroplane::throttle(float frameTime)
{
	plane->speed *= 0.998f;
}
