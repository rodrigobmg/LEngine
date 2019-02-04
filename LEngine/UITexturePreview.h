#pragma once
#ifndef _UITEXTUREPREVIEW_H_
#define _UITEXTUREPREVIEW_H_

#include "UIBase.h"
#include "d3dclass.h"
#include <shlobj.h>

class UITexturePreview : public UIBase
{
#define EMPTY_TEX L"NoTexture.dds"

public:
	///<summary> Initialize shape: Square </summary>
	bool Initialize(D3DClass* d3d, float centerX, float centerY, float size, wchar_t* textureFilename = L"");
	bool Initialize(D3DClass* d3d, float centerX, float centerY, float size, ID3D11Resource *&texture, ID3D11ShaderResourceView *&textureView, wchar_t* textureFilename = L"");
	void ConnectTextures(ID3D11Resource*& texture, ID3D11ShaderResourceView*& textureView);
	void TextureChooseWindow(HWND owner);
	void DeleteTexture();

	virtual bool SetShaderParameters(ID3D11DeviceContext *deviceContext, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix) override;
	virtual bool MouseOnArea(MouseClass* mouse);

private:
	void LoadNewTextureFromFile(wchar_t* textureFilename, bool onlyPreview = false);
	void ReleaseExternalTextures();
	void LoadExternalTextures(wchar_t* textureFilename);

private:
	D3DClass* m_D3D;
	ID3D11Resource* m_texture;
	ID3D11ShaderResourceView* m_textureView;

	ID3D11Resource** m_externalTexture;
	ID3D11ShaderResourceView** m_externalTextureView;

	float m_minX = 0, m_maxX = 0;
	float m_minY = 0, m_maxY = 0;
	float m_mousePosX, m_mousePosY;
};

#endif // !_UITEXTUREPREVIEW_H_