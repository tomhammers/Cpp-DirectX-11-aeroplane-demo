#include "MeshLoader.h"

MeshLoader::MeshLoader(ID3D11Device *device, ID3D11DeviceContext *context, 
	LPCTSTR meshLocation, int textureNumber, XMMATRIX &matProjection) {
	loadMesh(device, context, meshLocation, textureNumber, matProjection);
}

MeshLoader::~MeshLoader(){
	mesh.Destroy();
}

HRESULT MeshLoader::loadMesh(ID3D11Device *device,
	ID3D11DeviceContext *context, LPCTSTR meshLocation, int textureNumber, XMMATRIX &matProjection) {
	HRESULT hr = 0;

	V_RETURN(mesh.Create(device, meshLocation, true));

	ConstantBufferAndSampler *cbs = new ConstantBufferAndSampler();
	cbs->CreateConstandBuffersAndSampler(device);

	return S_OK;
}

void MeshLoader::renderMesh(ID3D11DeviceContext *context, XMMATRIX &worldViewProjection,
	XMMATRIX &matThingWorld, ID3D11Buffer *pcbVSPerObject, VertexAndPixelShaders *shaders) {

	CB_VS_PER_OBJECT CBMatrices;
	CBMatrices.matWorld = XMMatrixTranspose(matThingWorld);
	CBMatrices.matWorldViewProj = XMMatrixTranspose(worldViewProjection);
	context->UpdateSubresource(pcbVSPerObject, 0, NULL, &CBMatrices, 0, 0);
	context->VSSetConstantBuffers(0, 1, &pcbVSPerObject);

	context->IASetInputLayout(shaders->g_pVertexLayout11);
	UINT Strides[1];
	UINT Offsets[1];
	ID3D11Buffer* pVB[1];
	pVB[0] = mesh.GetVB11(0, 0);
	Strides[0] = (UINT)mesh.GetVertexStride(0, 0);
	Offsets[0] = 0;
	context->IASetVertexBuffers(0, 1, pVB, Strides, Offsets);
	context->IASetIndexBuffer(mesh.GetIB11(0), mesh.GetIBFormat11(0), 0);

	context->VSSetShader(shaders->g_pVertexShader, NULL, 0);
	context->PSSetShader(shaders->g_pPixelShader, NULL, 0);

	UINT                      numSubsets = mesh.GetNumSubsets(0);
	boolean                   mustRestoreOriginalTexture = false;
	ID3D11ShaderResourceView  *pDiffuseRV = NULL;
	SDKMESH_SUBSET			  *pSubset = NULL;
	D3D11_PRIMITIVE_TOPOLOGY  PrimType;

	//**************************************************************************//
	// Render each subset.														//
	//**************************************************************************//
	for (UINT subset = 0; subset < numSubsets; ++subset)
	{
		pSubset = NULL;
		// Get the subset
		pSubset = mesh.GetSubset(0, subset);

		PrimType = CDXUTSDKMesh::GetPrimitiveType11((SDKMESH_PRIMITIVE_TYPE)pSubset->PrimitiveType);
		context->IASetPrimitiveTopology(PrimType);

		//**************************************************************************//
		// Should there be more than one texture, we put it into each one into the  //
		// same slot in shader memory, which is going to be slow.					//
		//**************************************************************************//
		//if (subset > 0)	// not yet working
		{
			pDiffuseRV = mesh.GetMaterial(pSubset->MaterialID)->pDiffuseRV11;
			context->PSSetShaderResources(0, 1, &pDiffuseRV); // 0 was texturenumber
			mustRestoreOriginalTexture = true;
		}
		context->DrawIndexed((UINT)pSubset->IndexCount, 0, (UINT)pSubset->VertexStart);
	}

	//******************************************************************************//
	// Restore the original texture if we put another in its place.					//
	//******************************************************************************//
	if (mustRestoreOriginalTexture)
	{
		PrimType = CDXUTSDKMesh::GetPrimitiveType11((SDKMESH_PRIMITIVE_TYPE)pSubset->PrimitiveType);
		pSubset = mesh.GetSubset(0, 0);
		context->IASetPrimitiveTopology(PrimType);
		pDiffuseRV = mesh.GetMaterial(pSubset->MaterialID)->pDiffuseRV11;
		context->PSSetShaderResources(0, 1, &g_pTextureResourceView);
	}

	
}