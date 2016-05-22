#pragma once
#include <xnamath.h>
#include "ConstantBufferAndSampler.h"
#include "VertexAndPixelShaders.h"

class Thing
{
private:
	
	float x, y, z;        //Position.
	float rx, ry, rz;     //Rotation about x, y, z axes.
	float sx, sy, sz;     //Scales in x, y, z directions.
	float frameTime;

public:

	ID3D11InputLayout          *g_pVertexLayout11 = NULL;
	ID3D11Buffer               *g_pVertexBuffer = NULL;
	ID3D11Buffer               *g_pIndexBuffer = NULL;
	ID3D11Buffer               *g_ChangesEveryFrame = NULL;

	ID3D11Buffer               *g_pcbVSPerObject = NULL;
	ID3D11Buffer               *g_pcbPSPerObject = NULL;
	ID3D11Buffer               *g_pcbPSPerFrame = NULL;


	ID3D11DeviceContext*        g_pImmediateContext = NULL;
	ID3D11SamplerState*         g_pSamplerLinear = NULL;
	ID3D11ShaderResourceView   *g_pTextureResourceView = NULL;


	ID3D11Buffer               *g_pCBNeverChanges = NULL;
	ID3D11Buffer               *g_pCBChangeOnResize = NULL;
	ID3D11Buffer               *g_pCBChangesEveryFrame = NULL;

	//**************************************************************************//
	// This is a structure we pass to the vertex shader.  						//
	// Note we do it properly here and pass the WVP matrix, rather than world,	//
	// view and projection matrices separately.									//
	//**************************************************************************//
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



	struct CBChangesEveryFrame
	{
		XMMATRIX matWorldViewProjection;
		XMMATRIX matWorld;
	};



	struct CBNeverChanges
	{
		XMFLOAT4 materialColour;
		XMVECTOR vecLight;			// Must be 4, we only use the first 3.
	};

	struct CBChangeOnResize
	{
		XMMATRIX matProjection;
	};


public:
	float speed;
	Thing();
	~Thing();
	void init();

	XMVECTOR initialDir;
	XMVECTOR viewerCam;

	HRESULT getSpeed();
	HRESULT setSpeed(float speed);
	HRESULT setThrottleSpeed(float speed);

	HRESULT setPositionX(float x);
	HRESULT setPositionY(float y);
	HRESULT setPositionZ(float z);

	float getPositionX();
	float getPositionY();
	float getPositionZ();

	HRESULT moveThing(float x, float y, float z);

	HRESULT setRotationX(float rx);
	HRESULT setRotationY(float ry);
	HRESULT setRotationZ(float rz);

	float getRotationX();
	float getRotationY();
	float getRotationZ();

	HRESULT rotateThingX(float rx);
	HRESULT rotateThingY(float ry);
	HRESULT rotateThingZ(float rz);

	HRESULT setScaleX(float rx);
	HRESULT setScaleY(float ry);
	HRESULT setScaleZ(float rz);

	float getScaleX();
	float getScaleY();
	float getScaleZ();

	void setFrameTime(float frameTime);
	float getFrameTime();
};
