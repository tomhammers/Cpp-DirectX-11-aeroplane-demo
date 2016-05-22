#include "ObjLoader.h"

ObjLoader::ObjLoader() 
{
}



void ObjLoader::loadObj(LPSTR filename, ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX &matProjection, LPCWSTR texture)
{
	finalIndices = -1; // this is what will be pushed onto a vectorFinalIndices vector 
	fileStream.open(filename);

	while (std::getline(fileStream, line))
	{
		if (line.compare(0, 2, L"v ") == 0)  //"v space"
		{
			WCHAR first[5];
			float x, y, z;

			WCHAR oldStyleStr[200];
			wcscpy(oldStyleStr, line.c_str());
			swscanf(oldStyleStr, L"%2s%f%f%f", first, &x, &y, &z);

			VertexXYZ v;
			v.x = (float)x; v.y = (float)y; v.z = (float)z;
			vectorVertices.push_back(v);
		}

		if (line.compare(0, 3, L"vt ") == 0)  //"vt space"
		{
			WCHAR first[5];
			float u, v;

			WCHAR oldStyleStr[200];
			wcscpy(oldStyleStr, line.c_str());
			swscanf(oldStyleStr, L"%2s%f%f", first, &u, &v);

			VertexUV vt;
			vt.u = 1 - u; vt.v = 1 - v;
			vectorVerticesvt.push_back(vt);
		}

		if (line.compare(0, 3, L"vn ") == 0)  //"vn space"
		{
			WCHAR first[5];
			float a, b, c;

			WCHAR oldStyleStr[200];
			wcscpy(oldStyleStr, line.c_str());
			swscanf(oldStyleStr, L"%2s%f%f%f", first, &a, &b, &c);

			VertexXYZ vn;
			vn.x = a; vn.y = b; vn.z = c;
			vectorNormals.push_back(vn);
		}

		if (line.compare(0, 2, L"f ") == 0)  //"f space"
		{
			WCHAR first[5];
			WCHAR slash1[5];
			WCHAR slash2[5];
			WCHAR slash3[5];
			WCHAR slash4[5];
			WCHAR slash5[5];
			WCHAR slash6[5];

			UINT v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;

			WCHAR oldStyleStr[200];
			wcscpy(oldStyleStr, line.c_str());
			swscanf(oldStyleStr, L"%2s%d%1s%d%1s%d%d%1s%d%1s%d%d%1s%d%1s%d", first,
				&v1, slash1, &vt1, slash2, &vn1,
				&v2, slash3, &vt2, slash4, &vn2,
				&v3, slash5, &vt3, slash6, &vn3);

			SimpleVertex tempFace; // temporially put the data in here
			// FACE 1
			// using the index numbers of the face get the right co-ordinates
			tempFace.Pos.x = vectorVertices[v1 - 1].x;
			tempFace.Pos.y = vectorVertices[v1 - 1].y;
			tempFace.Pos.z = vectorVertices[v1 - 1].z;

			tempFace.TexUV.x = vectorVerticesvt[vt1 - 1].u;
			tempFace.TexUV.y = vectorVerticesvt[vt1 - 1].v;

			tempFace.VecNormal.x = vectorNormals[vn1 - 1].x;
			tempFace.VecNormal.y = vectorNormals[vn1 - 1].y;
			tempFace.VecNormal.z = vectorNormals[vn1 - 1].z;

			faces.push_back(tempFace); // now push the temp data onto a faces vector array

			finalIndices++;
			vectorFinalIndices.push_back(finalIndices);

			/////////////////////////
			// FACE 2
			tempFace.Pos.x = vectorVertices[v2 - 1].x;
			tempFace.Pos.y = vectorVertices[v2 - 1].y;
			tempFace.Pos.z = vectorVertices[v2 - 1].z;

			tempFace.TexUV.x = vectorVerticesvt[vt2 - 1].u;
			tempFace.TexUV.y = vectorVerticesvt[vt2 - 1].v;

			tempFace.VecNormal.x = vectorNormals[vn2 - 1].x;
			tempFace.VecNormal.y = vectorNormals[vn2 - 1].y;
			tempFace.VecNormal.z = vectorNormals[vn2 - 1].z;

			faces.push_back(tempFace);

			finalIndices++;
			vectorFinalIndices.push_back(finalIndices);

			////////////////////////////
			// FACE 3
			tempFace.Pos.x = vectorVertices[v3 - 1].x;
			tempFace.Pos.y = vectorVertices[v3 - 1].y;
			tempFace.Pos.z = vectorVertices[v3 - 1].z;

			tempFace.TexUV.x = vectorVerticesvt[vt3 - 1].u;
			tempFace.TexUV.y = vectorVerticesvt[vt3 - 1].v;

			tempFace.VecNormal.x = vectorNormals[vn3 - 1].x;
			tempFace.VecNormal.y = vectorNormals[vn3 - 1].y;
			tempFace.VecNormal.z = vectorNormals[vn3 - 1].z;

			faces.push_back(tempFace);

			finalIndices++;
			vectorFinalIndices.push_back(finalIndices);
		}
	}
	// call the buildmesh method
	buildMesh(vectorFinalIndices, faces, device, context, matProjection, texture);

}

int ObjLoader::buildMesh(std::vector <USHORT> vectorFinalIndices, 
	std::vector <SimpleVertex> faces, ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX &matProjection, LPCWSTR texture) {

	// can now use the pulled in data to build up a sortofmeshsubset struct
	// which can then pass the data to the buffer descriptor
	mesh->numVertices = faces.size();
	numberOfVertices = mesh->numVertices;
	mesh->vertices = new SimpleVertex[mesh->numVertices];

	for (int i = 0; i < mesh->numVertices; i++)
	{
		mesh->vertices[i].Pos.x = faces[i].Pos.x;
		mesh->vertices[i].Pos.y = faces[i].Pos.y;
		mesh->vertices[i].Pos.z = faces[i].Pos.z;

		mesh->vertices[i].TexUV.x = faces[i].TexUV.x;
		mesh->vertices[i].TexUV.y = faces[i].TexUV.y;

		mesh->vertices[i].VecNormal.x = faces[i].VecNormal.x;
		mesh->vertices[i].VecNormal.y = faces[i].VecNormal.y;
		mesh->vertices[i].VecNormal.z = faces[i].VecNormal.z;
	}

	mesh->numIndices = (USHORT)vectorFinalIndices.size();
	numberOfIndices = mesh->numIndices;
	mesh->indexes = new USHORT[mesh->numIndices];
	for (int i = 0; i < mesh->numIndices; i++)
	{
		mesh->indexes[i] = vectorFinalIndices[i];
	}


	HRESULT hr;
	D3D11_BUFFER_DESC bd;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * this->getnumVertices();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = this->mesh->vertices;

	hr = device->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(USHORT) * this->getNumberIndices();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = this->mesh->indexes;

	hr = device->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	g_numIndices = this->mesh->numIndices;

	// Set index buffer
	context->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	delete this->mesh->indexes;		// Delete the two arrays.
	delete this->mesh->vertices;
	delete this->mesh;

	//**************************************************************************//
	// Create the 3 constant buffers.											//
	//**************************************************************************//
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBNeverChanges);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
	if (FAILED(hr))
		return hr;

	bd.ByteWidth = sizeof(CBChangeOnResize);
	hr = device->CreateBuffer(&bd, NULL, &g_pCBChangeOnResize);
	if (FAILED(hr))
		return hr;

	bd.ByteWidth = sizeof(CBChangesEveryFrame);
	hr = device->CreateBuffer(&bd, NULL, &g_pCBChangesEveryFrame);
	if (FAILED(hr))
		return hr;

	//**************************************************************************//
	// Load the texture into "ordinary" RAM.									//
	//**************************************************************************//
	hr = D3DX11CreateShaderResourceViewFromFile(device, texture,
		NULL, NULL, &g_pTextureResourceView, NULL);
	if (FAILED(hr))
		return hr;

	ConstantBufferAndSampler *cbs = new ConstantBufferAndSampler();
	cbs->CreateConstandBuffersAndSampler(device);


	////**************************************************************************//
	//// Update the constant buffer for stuff (the light vector and material		//
	//// colour in this case) that never change.  This is faster; don't update	//
	//// stuff if you don't have to.												//
	////**************************************************************************//
	CBNeverChanges cbNeverChanges;
	cbNeverChanges.materialColour = XMFLOAT4(1, 1, 1, 1);		//Alpha does nothing.
	cbNeverChanges.vecLight = XMVectorSet(1, 1, -2, 0);	//4th value unused.
	cbNeverChanges.vecLight = XMVector3Normalize(cbNeverChanges.vecLight);
	context->UpdateSubresource(g_pCBNeverChanges,
		0, NULL,
		&cbNeverChanges,
		0, 0);


	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.matProjection = XMMatrixTranspose(matProjection);
	context->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
}


void ObjLoader::renderMesh(ID3D11DeviceContext *context, XMMATRIX &worldViewProjection, 
	XMMATRIX &matThingWorld, XMMATRIX &matProjection, ID3D11Buffer *pcbVSPerObject, VertexAndPixelShaders *shaders)
{
	// this will render the mesh every frame
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	context->IASetInputLayout(shaders->g_pVertexLayout11);
	context->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//**************************************************************************//
	// At the moment we load a texture into video memory every frame, which is	//
	// HORRIBLE, we need to create more Texture2D variables.					//
	//**************************************************************************//
	context->PSSetShaderResources(0, 1, &g_pTextureResourceView);


	context->PSSetConstantBuffers(0, 1, &g_pCBNeverChanges);		// Note this one belongs to the pixel shader.
	context->VSSetConstantBuffers(0, 1, &g_pCBChangeOnResize);		// Paremeter 1 relates to pisition in 
	context->VSSetConstantBuffers(1, 1, &g_pCBChangesEveryFrame);	// constant buffers.
	context->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
	context->PSSetSamplers(0, 1, &g_pSamplerLinear);
	
	CBChangesEveryFrame cb;
	cb.matWorld = XMMatrixTranspose(matThingWorld);
	cb.matWorldViewProjection = XMMatrixTranspose(worldViewProjection);

	context->UpdateSubresource(pcbVSPerObject, 0, NULL, &cb, 0, 0);
	context->VSSetConstantBuffers(0, 1, &pcbVSPerObject);
	context->DrawIndexed(g_numIndices, 0, 0);
}

int ObjLoader::getnumVertices() {
	return numberOfVertices;
}

int ObjLoader::getNumberIndices() {
	return numberOfIndices;
}


ObjLoader::~ObjLoader() {
	if (g_pVertexLayout11) g_pVertexLayout11->Release();
	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pIndexBuffer) g_pIndexBuffer->Release();
	if (g_ChangesEveryFrame) g_ChangesEveryFrame->Release();
	if (g_pcbVSPerObject) g_pcbVSPerObject->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pSamplerLinear) g_pSamplerLinear->Release();
	if (g_pTextureResourceView) g_pTextureResourceView->Release();
	if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
	if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
	if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
}


