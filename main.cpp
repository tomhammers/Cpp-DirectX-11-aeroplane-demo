//**************************************************************************//
// Modifications to the MS sample code is copyright of Dr Nigel Barlow,		//
// lecturer in computing, University of Plymouth, UK.						//
// email: nigel@soc.plymouth.ac.uk.											//
//																			//
// Sdkmesh added to MS sample Tutorial09.									//
//																			//
// You may use, modify and distribute this (rather cack-handed in places)	//
// code subject to the following conditions:								//
//																			//
//	1:	You may not use it, or sell it, or use it in any adapted form for	//
//		financial gain, without my written premission.						//
//																			//
//	2:	You must not remove the copyright messages.							//
//																			//
//	3:	You should correct at least 10% of the typing abd spekking errirs.  //
//	"I corerected one of them"
//**************************************************************************//

//--------------------------------------------------------------------------------------
// By Thomas Kerr ~ starting point was Nigel's adapted Microsoft sample:
// Tutorial 09 - Meshes using DXUT helper Classes.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#define _WINSOCKAPI_    // stops windows.h including winsock.h .............. and many errors!

#include <Windows.h>
#include <stdio.h>
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;

#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTgui.h"
#include "DXUTsettingsDlg.h"
#include "SDKmisc.h"
#include "SDKMesh.h"
#include <Windows.h>
#include <xnamath.h>
#include <winsock2.h>
#include "resource.h"

#include "ObjLoader.h"
#include "MeshLoader.h"
#include "WiiMoteInfo.h"
#include "ConstantBufferAndSampler.h"
#include "VertexAndPixelShaders.h"
#include "ConnectToNetwork.h"
#include "Aeroplane.h"

Aeroplane					*Player1Plane;
Aeroplane					*Player2Plane;
MeshLoader					*SkyBox;
MeshLoader					*Island;

ConnectToNetwork			*network;
bool						someoneIsHere = false;

POINT						cursor;
float						cursPercX;
float						cursPercY;
WiiMoteInfo					*wiimote;
ConstantBufferAndSampler	*cb_sampler;
VertexAndPixelShaders		*v_p_shaders;
VertexAndPixelShaders		*sky_shaders;

//**************************************************************************//
// Now a global instance of each constant buffer.							//
//**************************************************************************//
ID3D11Buffer                       *g_pCBNeverChanges = NULL;
ID3D11Buffer                       *g_pCBChangeOnResize = NULL;
ID3D11Buffer                       *g_pCBChangesEveryFrame = NULL;

ID3D11Device*                       g_pd3dDevice = NULL;
ID3D11DeviceContext*                g_pImmediateContext = NULL;

//**************************************************************************//
// Global Variables.  There are many global variables here (we aren't OO	//
// yet.  I doubt  Roy Tucker (Comp Sci students will know him) will			//
// approve pf this either.  Sorry, Roy.										//
//**************************************************************************//
CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg             g_D3DSettingsDlg;       // Device settings dialog
CDXUTDialog                 g_HUD;                  // manages the 3D   
CDXUTDialog                 g_SampleUI;             // dialog for sample specific controls

int							g_width  = 1200;
int							g_height = 900;

XMMATRIX					g_MatProjection;
// control movement
bool		 g_b_LeftArrowDown      = false;	//Status of keyboard.  Thess are set
bool		 g_b_RightArrowDown     = false;	//in the callback KeyboardProc(), and 
bool		 g_b_UpArrowDown	    = false;	//are used in onFrameMove().
bool		 g_b_DownArrowDown	    = false;
bool		 g_bWKeyDown			= false;
bool		 g_bSKeyDown = false;

bool		 g_b_LeftMouseClick		= false;

XMVECTOR g_vecPlaneInitialDir = XMVectorSet(0, 0, -1, 0);
XMVECTOR localDir = XMVectorSet(0, 0, -1, 0);
XMVECTOR viewerCam = XMVectorSet(0, 0, -1, 0);

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

//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext );
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext);
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext );
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext );
void CALLBACK OnD3D11DestroyDevice( void* pUserContext );
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
                                  float fElapsedTime, void* pUserContext );
void InitApp();
//**************************************************************************//
// A Windows program always kicks off in WinMain.							//
// Initializes everything and goes into a message processing				//
// loop.																	//
//																			//
// This version uses DXUT, and is much more complicated than previous		//
// versions you have seen.  This allows you to sync the frame rate to your  //
// monitor's vertical sync event.											//
//**************************************************************************//
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	//**************************************************************************//
	// Set DXUT callbacks.														//
    //**************************************************************************//
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( OnKeyboard );
	DXUTSetCallbackFrameMove(OnFrameMove);
    DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
    DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
    DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );
    DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
    DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );

    InitApp();
    DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"Wii Plane - Soft323" );
    DXUTCreateDevice (D3D_FEATURE_LEVEL_9_2, true, 1200, 900 );

	// Code added to play sound continously while the application is running
	PlaySound(L"Media\\aircraft.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	// attempt to connect to the network
	network = new ConnectToNetwork();

    DXUTMainLoop(); // Enter into the DXUT render loop
    return DXUTGetExitCode();
}

//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    // Initialize dialogs
    g_D3DSettingsDlg.Init( &g_DialogResourceManager );
    g_HUD.Init( &g_DialogResourceManager );
    g_SampleUI.Init( &g_DialogResourceManager );
}

void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	// receive latest co-ordinates, if a plaer is there
	network->receive(Player2Plane);

	wiimote = new WiiMoteInfo();
	wiimote->setWiiMoteData();

	// Arrow keys
	if (g_b_UpArrowDown) Player1Plane->plane->setSpeed(150);
	if (!g_b_UpArrowDown && !g_b_DownArrowDown) Player1Plane->plane->setSpeed(20);
	if (g_b_DownArrowDown) Player1Plane->plane->setSpeed(0);
	// stops the plane locking at a wrong angle
	if (!g_b_LeftArrowDown && !g_b_RightArrowDown) Player1Plane->plane->rotateThingZ(0);

	// Mouse movement
	if (g_b_LeftMouseClick){
		if (cursor.x < 601) // mouse is on left half of window
		{
			cursPercX = ((float)cursor.x / 600 * 100) - 100;
			cursPercX = fabsf(cursPercX); // turning a negative into a positive

			Player1Plane->plane->rotateThingZ(-(cursPercX / 100 * 0.785398)); // 45 degrees in radian 0.785398
			Player1Plane->plane->rotateThingY(-fElapsedTime * (cursPercX / 100));
		}
		else {				// mouse is on right half of window
			cursPercX = ((float)cursor.x - 600);
			cursPercX = cursPercX / 600 * 100;
			cursPercX = fabsf(cursPercX);

			Player1Plane->plane->rotateThingZ(cursPercX / 100 * 0.785398);
			Player1Plane->plane->rotateThingY(fElapsedTime * (cursPercX / 100));
		}

		if (cursor.y < 451) // mouse is in top half of window
		{
			cursPercY = ((float)cursor.y / 450 * 100) - 100;
			cursPercY = fabsf(cursPercY); // turning a negative into a positive

			Player1Plane->plane->rotateThingX(-(cursPercY / 100 * 0.785398));
		}
		else {				// mouse is in bottom half of window
			cursPercY = ((float)cursor.y / 450 * 100) - 100;
			cursPercY = fabsf(cursPercY); // turning a negative into a positive

			Player1Plane->plane->rotateThingX(cursPercY / 100 * 0.785398);
		}
	}

	// WiiMote
	if (wiimote->buttonAPressed == true)
	{
		// convienetently the wiimote gave +/- numbers on each axis that could be plugged straight into the planes methods
		Player1Plane->plane->rotateThingZ(wiimote->roll);
		Player1Plane->plane->rotateThingY(fElapsedTime * wiimote->roll);

		Player1Plane->plane->rotateThingX(-wiimote->pitch * 0.5);
	}

	// now send plane position to the server, it seems logical to send it right away?
	network->sendMessage(Player1Plane->plane->getPositionX(), Player1Plane->plane->getPositionY(), Player1Plane->plane->getPositionZ(),
		0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{	
	switch (uMsg)
	{
		// catch mouse move messages, store them for use 
	case WM_MOUSEMOVE:
		cursor.x = LOWORD(lParam);
		cursor.y = HIWORD(lParam);	
		break;
		// is left mouse button down?
	case WM_LBUTTONDOWN:
		// then put cursor in middle of screen (take control of aeroplane)
		cursor.x = 600;
		cursor.y = 450;
		ClientToScreen(hWnd, &cursor);	  // put cursor on window
		SetCursorPos(cursor.x, cursor.y); // sets the cursor in center of screen
		g_b_LeftMouseClick = true;
		break;
	case WM_LBUTTONUP:
		g_b_LeftMouseClick = false;
	}

    // Pass messages to dialog resource manager calls so GUI state is updated correctly
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    // Pass messages to settings dialog if its active
    if( g_D3DSettingsDlg.IsActive() )
    {
        g_D3DSettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }
    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    return 0;
}

//**************************************************************************//
// Handle key presses.														//
//**************************************************************************//
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	switch( nChar )
	{		       
		case VK_LEFT:  g_b_LeftArrowDown  = bKeyDown; break;
		case VK_RIGHT: g_b_RightArrowDown = bKeyDown; break;
		case VK_UP:    g_b_UpArrowDown    = bKeyDown; break;
		case VK_DOWN:  g_b_DownArrowDown  = bKeyDown; break;
		case 0x57:	   g_bWKeyDown = bKeyDown; break;
		case 0x53:	   g_bSKeyDown = bKeyDown; break;
       }
}


//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    return true;
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext )
{
    HRESULT hr;
    ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();
    V_RETURN( g_DialogResourceManager.OnD3D11CreateDevice( pd3dDevice, pd3dImmediateContext ) );
    V_RETURN( g_D3DSettingsDlg.OnD3D11CreateDevice( pd3dDevice ) );

	v_p_shaders = new VertexAndPixelShaders();
	// Compile and create a Vertex Shader
	v_p_shaders->CreateVertexShader(pd3dDevice);
	// Compile and create a Pixel Shader
	v_p_shaders->CreatePixelShader(pd3dDevice);
	// Create the input layout
	v_p_shaders->MakeInputLayout(pd3dDevice);

    //****************************************************************************************************************//
	// Initialize the projection matrix.  Generally you will only want to create this matrix once and then forget it.										
	//****************************************************************************************************************//
	g_MatProjection = XMMatrixPerspectiveFovLH( XM_PIDIV2,				 // Field of view (pi / 2 radians, or 90 degrees
											 g_width / (FLOAT) g_height, // Aspect ratio.
											 0.01f,						 // Near clipping plane.
											 10000.0f );				 // Far clipping plane.

	// build our own aeroplane
	Player1Plane = new Aeroplane();
	Player1Plane->buildPlane(pd3dDevice, pd3dImmediateContext, g_MatProjection);

	// if we are connected build another aeroplane for potentially another player
	Player2Plane = new Aeroplane();
	Player2Plane->buildPlane(pd3dDevice, pd3dImmediateContext, g_MatProjection);
	
    // Load MeshLoader objects for sdkmesh														
	Island = new MeshLoader(pd3dDevice, pd3dImmediateContext, L"Media\\IslandScene\\island.sdkmesh", 0, g_MatProjection);

	SkyBox = new MeshLoader(pd3dDevice, pd3dImmediateContext, L"Media\\SkyBox\\skysphere.sdkmesh", 0, g_MatProjection);

	// constant buffer and sampler is created here
	cb_sampler = new ConstantBufferAndSampler();
	cb_sampler->CreateConstandBuffersAndSampler(pd3dDevice);
	
    return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;
    V_RETURN( g_DialogResourceManager.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );
    V_RETURN( g_D3DSettingsDlg.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );

	g_width  = pBackBufferSurfaceDesc->Width;
	g_height = pBackBufferSurfaceDesc->Height;

	g_HUD.SetLocation( g_width - 170, 0 );
    g_HUD.SetSize( 170, 170 );
	g_SampleUI.SetLocation( g_width - 170, g_height - 300 );
    g_SampleUI.SetSize( 170, 300 );

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
                                  float fElapsedTime, void* pUserContext )
{
    HRESULT hr;

    // Clear the render target and depth stencil
    float ClearColor[4] = { 0.0f, 0.25f, 0.25f, 0.55f };
    ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
    pd3dImmediateContext->ClearRenderTargetView( pRTV, ClearColor );
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
    pd3dImmediateContext->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH, 1.0, 0 );

	//**************************************************************************//
    // Initialize the view matrix.  What you do to the viewer matrix moves the  //
	// viewer, or course.														//																		
	// The viewer matrix is created every frame here, which looks silly as the	//
	// viewer never moves.  However in general your viewer does move.			//
	//**************************************************************************//
	XMVECTOR Eye = XMVectorSet(Player1Plane->plane->getPositionX() + XMVectorGetX(viewerCam), Player1Plane->plane->getPositionY() + 4.5 + XMVectorGetY(viewerCam), Player1Plane->plane->getPositionZ() + XMVectorGetZ(viewerCam), 0.0f);
	XMVECTOR At = XMVectorSet(Player1Plane->plane->getPositionX(), Player1Plane->plane->getPositionY(), Player1Plane->plane->getPositionZ(), 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX matView;
	matView = XMMatrixLookAtLH(Eye,	// The eye, or viewer's position.
		At,		// The look at point.
		Up);	// Which way is up.

	// Need this here for player 1 or everything breaks, used for plane direction and viewer cam
	XMMATRIX matPlaneRotate = XMMatrixRotationRollPitchYaw(Player1Plane->plane->getRotationX(), Player1Plane->plane->getRotationY(), Player1Plane->plane->getRotationZ());

	// island
	XMMATRIX matIslandTranslate = XMMatrixTranslation(0, -200, 0);
	XMMATRIX matIslandScale = XMMatrixScaling(1, 1, 1);
	XMMATRIX matIslandWorld = matIslandTranslate * matIslandScale;
	XMMATRIX matIslandWorldViewProjection = matIslandWorld * matView * g_MatProjection;

	// sky
	XMMATRIX matSkyTranslate = XMMatrixTranslation(Player1Plane->plane->getPositionX(), Player1Plane->plane->getPositionY(), Player1Plane->plane->getPositionZ());
	XMMATRIX matSkyScale = XMMatrixScaling(1, 1, 1);
	XMMATRIX matSkyWorld = matSkyTranslate * matSkyScale;
	XMMATRIX matSkyWorldViewProjection = matSkyWorld * matView * g_MatProjection;

	// plane direction
	XMVECTOR vecNewDir;
	vecNewDir = XMVector3TransformCoord(g_vecPlaneInitialDir, matPlaneRotate);
	vecNewDir = XMVector3Normalize(vecNewDir);
	vecNewDir *= Player1Plane->plane->getSpeed() * fElapsedTime;
	Player1Plane->plane->moveThing(XMVectorGetX(vecNewDir), XMVectorGetY(vecNewDir), XMVectorGetZ(vecNewDir));

	// viewer cam
	viewerCam = XMVector3TransformCoord(localDir, matPlaneRotate);
	viewerCam = XMVector3Normalize(viewerCam);
	viewerCam *= -14.0;

	//******************************************************************//
	// Lighting.  Ambient light and a light direction, above, to the	//
	// left and two paces back, I think.  Then normalise the light		//
	// vector.  It is kind-a-silly doing this every frame unless the	//
	// light moves.														//
	//******************************************************************//
    float    fAmbient                = 0.1f;
	XMVECTOR vectorLightDirection    = XMVectorSet(-1, 1, -1, 0);  // 4th value unused.
	vectorLightDirection = XMVector3Normalize(vectorLightDirection);
										
	CB_PS_PER_FRAME CBPerFrame;
	CBPerFrame.m_vLightDirAmbient = vectorLightDirection;
	pd3dImmediateContext->UpdateSubresource(cb_sampler->g_pcbPSPerFrame, 0, NULL, &CBPerFrame, 0, 0);
	pd3dImmediateContext->PSSetConstantBuffers(1, 1, &cb_sampler->g_pcbPSPerFrame);

	CB_PS_PER_OBJECT CBPerObject;
	CBPerObject.m_vObjectColor = XMFLOAT4(1, 1, 1, 1);
	pd3dImmediateContext->UpdateSubresource(cb_sampler->g_pcbPSPerObject, 0, NULL, &CBPerObject, 0, 0);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &cb_sampler->g_pcbPSPerObject);
	pd3dImmediateContext->PSSetSamplers(0, 1, &cb_sampler->g_pSamplerLinear);

	Island->renderMesh(pd3dImmediateContext, matIslandWorldViewProjection, matIslandWorld, cb_sampler->g_pcbVSPerObject, v_p_shaders);
	SkyBox->renderMesh(pd3dImmediateContext, matSkyWorldViewProjection, matSkyWorld, cb_sampler->g_pcbVSPerObject, v_p_shaders);

	Player1Plane->renderPlane(pd3dImmediateContext, matView, g_MatProjection, cb_sampler->g_pcbVSPerObject, v_p_shaders);
	Player2Plane->renderPlane(pd3dImmediateContext, matView, g_MatProjection, cb_sampler->g_pcbVSPerObject, v_p_shaders);
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{
    g_DialogResourceManager.OnD3D11ReleasingSwapChain();
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
    g_DialogResourceManager.OnD3D11DestroyDevice();
    g_D3DSettingsDlg.OnD3D11DestroyDevice();
    DXUTGetGlobalResourceCache().OnDestroyDevice();
	// clean up the objects
	Island->~MeshLoader();
	SkyBox->~MeshLoader();
	Player1Plane->plane->~ObjLoader();
	Player1Plane->propellor->~ObjLoader();
	Player2Plane->plane->~ObjLoader();
	Player2Plane->propellor->~ObjLoader();
	cb_sampler->~ConstantBufferAndSampler();
	v_p_shaders->~VertexAndPixelShaders();
	network->~ConnectToNetwork();
}