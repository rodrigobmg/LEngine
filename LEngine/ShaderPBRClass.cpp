#include "ShaderPBRClass.h"

void ShaderPBRClass::SetRoughness(float roughness)
{
	m_roughness = roughness;
}

void ShaderPBRClass::SetMetalness(float metalness)
{
	m_metalness = metalness;
}

bool ShaderPBRClass::CreateBufferAdditionals(ID3D11Device * &device)
{
	BaseShaderClass::CreateBufferAdditionals(device);
	D3D11_BUFFER_DESC tempBufferDesc;

	tempBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tempBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tempBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tempBufferDesc.MiscFlags = 0;
	tempBufferDesc.StructureByteStride = 0;

	tempBufferDesc.ByteWidth = sizeof(LightingBufferType);
	if (FAILED(device->CreateBuffer(&tempBufferDesc, NULL, &m_lightingBuffer)))
		return false;

	tempBufferDesc.ByteWidth = sizeof(CameraBufferType);
	if (FAILED(device->CreateBuffer(&tempBufferDesc, NULL, &m_cameraBuffer)))
		return false;

	tempBufferDesc.ByteWidth = sizeof(PBRBufferType);
	if (FAILED(device->CreateBuffer(&tempBufferDesc, NULL, &m_PBRBuffer)))
		return false;

	m_buffers = { m_lightingBuffer, m_cameraBuffer, m_PBRBuffer };

	return true;
}

bool ShaderPBRClass::SetShaderParameters(ID3D11DeviceContext *deviceContext, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix)
{
	if (BaseShaderClass::SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix) == false)
		return false;

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	LightingBufferType* dataPtr2;
	CameraBufferType* dataPtr3;
	PBRBufferType* dataPtr4;
	unsigned int bufferNumber;

	/////// VERTEX BUFFERS ///////
	//Camera buffer
	result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	dataPtr3 = (CameraBufferType*)mappedResource.pData;
	dataPtr3->cameraDirection = m_cameraPosition;
	dataPtr3->padding = 0;

	deviceContext->Unmap(m_cameraBuffer, 0);
	bufferNumber = 1;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);

	/////// PIXEL BUFFERS ///////
	//Lighting buffer
	result = deviceContext->Map(m_lightingBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	dataPtr2 = (LightingBufferType*)mappedResource.pData;
	dataPtr2->direction = m_lightDirection;
	dataPtr2->padding = 0;

	deviceContext->Unmap(m_lightingBuffer, 0);
	bufferNumber = 0;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightingBuffer);

	//PBR Buffer
	result = deviceContext->Map(m_PBRBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	dataPtr4 = (PBRBufferType*)mappedResource.pData;
	dataPtr4->roughness = m_roughness;
	dataPtr4->metalness = m_metalness;
	dataPtr4->padding = XMFLOAT2{ 0,0 };

	deviceContext->Unmap(m_PBRBuffer, 0);
	bufferNumber = 1;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_PBRBuffer);

	/////// RESOURCES ///////
	//Pixel shader resources
	deviceContext->PSSetShaderResources(0, 1, &m_diffuseTextureView);
	deviceContext->PSSetShaderResources(1, 1, &m_normalTextureView);
	return true;
}