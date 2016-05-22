#pragma once
#include <Windows.h>
#include <xnamath.h>
#include <vector>
#include <fstream>
#include <string>	
#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTgui.h"
#include "Thing.h"

// I'm not totally pleased with the structure of this class
// But I got everything working so it stays put for now!

class ObjLoader : public Thing{
public:
	struct SimpleVertex
	{
		XMFLOAT3 Pos;	//Why not a float4?  See the shader strucrure.  Any thoughts?  Nigel
		XMFLOAT3 VecNormal;
		XMFLOAT2 TexUV;
	};

	struct SortOfMeshSubset
	{
		SimpleVertex *vertices;
		USHORT       *indexes;
		USHORT       numVertices;
		USHORT       numIndices;
	};

	struct VertexXYZ
	{
		float x, y, z;
	};

	struct VertexUV
	{
		float u, v;
	};

	struct CBChangeOnResize
	{
		XMMATRIX matProjection;
	};

	SortOfMeshSubset *mesh = new SortOfMeshSubset;


	ObjLoader();
	~ObjLoader();


	void loadObj(LPSTR filename, ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX &matProjection, LPCWSTR texture);

	int buildMesh(std::vector <USHORT> vectorFinalIndices, std::vector <SimpleVertex> faces, 
		ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX &matProjection, LPCWSTR texture);
	
	void renderMesh(ID3D11DeviceContext *context, XMMATRIX &worldViewProjection, 
		XMMATRIX &matThingWorld, XMMATRIX &matProjection, ID3D11Buffer *pcbVSPerObject, VertexAndPixelShaders *shaders);
	
	// had good intentions at some point
	int getnumVertices();
	int getNumberIndices();

private:
	std::wifstream				fileStream;
	std::wstring				line;
	std::vector <VertexXYZ>		vectorVertices;
	std::vector <VertexXYZ>		vectorNormals;
	std::vector <VertexUV>		vectorVerticesvt;
	std::vector <USHORT>		vectorFinalIndices;


	int	  finalIndices;
	int   numberOfVertices;
	int	  numberOfIndices;
	int   g_numIndices;
	std::vector <SimpleVertex> faces;
};