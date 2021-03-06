#pragma once
#ifndef _RENDERTEXTURECLASS_H_
#define _RENDERTEXTURECLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <string>
using namespace DirectX;
using namespace std;

class RenderTextureClass
{
public:
	enum Scaling{
		NONE, UPSCALE, DOWNSCALE
	};

public:
	RenderTextureClass();

	bool InitializeShadowMap(ID3D11Device* device, int textureWidth, int textureHeight);
	bool Initialize(ID3D11Device* device, int textureWidth, int textureHeight, RenderTextureClass::Scaling scaling = NONE, bool skybox = false);
	bool Initialize(ID3D11Device* device, int textureWidth, int textureHeight, int mipLevels);
	void Shutdown();

	///<summary>Choose target to pass render info</summary>
	void SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView, bool withViewport = true);
	///<summary>Clear target every time before usage!</summary>
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView, float red, float green, float blue, float alpha);
	void SetViewport(ID3D11DeviceContext * deviceContext);
	///<summary>Non-DDS textures require different loader - choose if your data is in DDS format</summary>
	bool LoadTexture(ID3D11Device * device, const wchar_t* filename, ID3D11Resource *&m_texture, ID3D11ShaderResourceView *&m_textureView, bool isDDS = true);
	ID3D11ShaderResourceView*& GetShaderResourceView();
	ID3D11RenderTargetView*& GetShaderTargetView(int skyboxIndex);
	ID3D11Resource*& GetShaderResource();
	void GetOrthoMatrix(XMMATRIX &orthoMatrix);

private:
	bool Initialize2DTexture(ID3D11Device*& device, int textureWidth, int textureHeight, RenderTextureClass::Scaling scaling);
	bool InitializeSkybox(ID3D11Device*& device, int textureWidth, int textureHeight, RenderTextureClass::Scaling scaling);

private:
	XMMATRIX m_orthoMatrix;
	D3D11_VIEWPORT m_viewport;

	ID3D11Texture2D *m_texture2D;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11RenderTargetView* m_renderTargetViewSkybox[6];
	ID3D11ShaderResourceView* m_shaderResourceView;
};
#endif // !_RENDERTEXTURECLASS_H_