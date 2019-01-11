#pragma once
#ifndef _BASESHADERCLASS_H_
#define _BASESHADERCLASS_H_

#pragma comment(lib, "d3dcompiler.lib")

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DDSTextureLoader.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>
#include <vector>
using namespace std;
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: ColorShaderClass
////////////////////////////////////////////////////////////////////////////////
class BaseShaderClass
{
public:
	BaseShaderClass();
	BaseShaderClass(const BaseShaderClass&);
	~BaseShaderClass();

	bool Initialize(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX&, XMMATRIX&, XMMATRIX&);
	bool LoadTexture(ID3D11Device * device, const wchar_t* filename, ID3D11Resource *&m_texture, ID3D11ShaderResourceView *&m_textureView);

protected:
	virtual bool CreateInputLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer);
	virtual bool CreateBuffers(ID3D11Device * device);
	virtual bool CreateSamplerState(ID3D11Device* device);
	virtual void ShutdownShader();
	virtual bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX&, XMMATRIX&, XMMATRIX&);
	virtual void RenderShader(ID3D11DeviceContext*, int);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, CHAR*);

protected:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_samplerState;
};

#endif