#include "UITexturePreview.h"

bool UITexturePreview::Initialize(D3DClass * d3d, float centerX, float centerY, float size, wchar_t* textureFilename)
{
	m_D3D = d3d;
	if (!BaseShaderClass::Initialize(d3d->GetDevice(), *d3d->GetHWND(), L"uiTexturePreview.vs", L"uiTexturePreview.ps", BaseShaderClass::vertexInputType(GetInputNames(), GetInputFormats())))
		return false;

	if (textureFilename == L"")
	{
		if (m_externalTexture != nullptr && m_externalTextureView != nullptr)
		{
			m_texture = *m_externalTexture;
			m_textureView = *m_externalTextureView;
		}
		else
			LoadNewTextureFromFile(textureFilename, true);
	}
	else
	{
		BaseShaderClass::LoadTexture(d3d->GetDevice(), textureFilename, m_texture, m_textureView);
		LoadExternalTextures(textureFilename);
	}

	m_minX = centerX - size / 2.0f;
	m_maxX = centerX + size / 2.0f;
	m_minY = centerY - size / 2.0f;
	m_maxY = centerY + size / 2.0f;

	return InitializeSquare(d3d->GetDevice(), centerX, centerY, size, false, true);
}

bool UITexturePreview::Initialize(D3DClass * d3d, float centerX, float centerY, float size, ID3D11Resource *& texture, ID3D11ShaderResourceView *& textureView, wchar_t * textureFilename)
{
	m_D3D = d3d;
	ConnectTextures(texture, textureView);

	if (!Initialize(d3d, centerX, centerY, size, textureFilename))
		return false;
	
	return true;
}

void UITexturePreview::ConnectTextures(ID3D11Resource *& texture, ID3D11ShaderResourceView *& textureView)
{
	m_externalTexture = &texture;
	m_externalTextureView = &textureView;
}

void UITexturePreview::TextureChooseWindow(HWND hwnd)
{
	PWSTR pszFilePath;
	wchar_t* wFilePath = 0;
	IFileOpenDialog *pFileOpen;
	const COMDLG_FILTERSPEC rgSpec[] = { L"DDS (DirectDraw Surface)", L"*.dds" };
	// Create the FileOpenDialog object.
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr))
		hr = pFileOpen->SetFileTypes(1, rgSpec);

	if (SUCCEEDED(hr))
	{
		// Show the Open dialog box.
		hr = pFileOpen->Show(NULL);

		// Get the file name from the dialog box.
		if (SUCCEEDED(hr))
		{
			IShellItem *pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				// Display the file name to the user.
				if (SUCCEEDED(hr))
				{
					wFilePath = pszFilePath;
					LoadNewTextureFromFile(wFilePath);
					CoTaskMemFree(pszFilePath);

				}
				pItem->Release();
			}
		}
		pFileOpen->Release();
	}

	//Do not call - because it will disallow using open dialog again
		//CoUninitialize();
}

void UITexturePreview::DeleteTexture()
{
	BaseShaderClass::LoadTexture(m_D3D->GetDevice(), EMPTY_TEX, m_texture, m_textureView);
	ReleaseExternalTextures();
}

bool UITexturePreview::SetShaderParameters(ID3D11DeviceContext * deviceContext, XMMATRIX & worldMatrix, XMMATRIX & viewMatrix, XMMATRIX & projectionMatrix)
{
	deviceContext->PSSetShaderResources(0, 1, &m_textureView);

	return UIBase::SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
}

bool UITexturePreview::MouseOnArea(MouseClass * mouse)
{
	mouse->GetMouseLocationScreenSpace(m_mousePosX, m_mousePosY);

	return (m_mousePosX >= m_minX && m_mousePosX <= m_maxX &&
		m_mousePosY >= m_minY && m_mousePosY <= m_maxY);
}

void UITexturePreview::LoadNewTextureFromFile(wchar_t* textureFilename, bool onlyPreview)
{
	if (!BaseShaderClass::LoadTexture(m_D3D->GetDevice(), textureFilename, m_texture, m_textureView))
	{
		if (!BaseShaderClass::LoadTexture(m_D3D->GetDevice(), EMPTY_TEX, m_texture, m_textureView))
		{
			MessageBox(*m_D3D->GetHWND(), "UITexturePreview -> Loading texture error", "Texture Loading Error", MB_OK);
			PostQuitMessage(0);
		}
	}
	if (onlyPreview == false)
		LoadExternalTextures(textureFilename);
}

void UITexturePreview::ReleaseExternalTextures()
{
	if (m_externalTexture != nullptr && *m_externalTexture != NULL)
	{
		(*m_externalTexture)->Release();
		*m_externalTexture = nullptr;
	}
	if (m_externalTextureView != nullptr && *m_externalTextureView != NULL)
	{
		(*m_externalTextureView)->Release();
		*m_externalTextureView = nullptr;
	}
}

void UITexturePreview::LoadExternalTextures(wchar_t * textureFilename)
{
	if (m_externalTexture != nullptr && m_externalTextureView != nullptr)
	{
		BaseShaderClass::LoadTexture(m_D3D->GetDevice(), textureFilename, *m_externalTexture, *m_externalTextureView);
	}
}