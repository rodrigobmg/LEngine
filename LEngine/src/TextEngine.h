#pragma once
#pragma warning(disable:4996)
#ifndef _TEXT_ENGINE_H
#define _TEXT_ENGINE_H

#include <d3d11.h>
#include <SpriteFont.h>
#include <DirectXMath.h>
#include <vector>
#include "d3dclass.h"

///////////////////////////////////////////////////////////////////
// PROPABLY WILL BE REPLACED BY dxguid.lib (or slightly modified) //
///////////////////////////////////////////////////////////////////

///<summary>Collects all text data and render it once at the end of frame</summary>
class TextEngine
{
public:
	enum Align{ LEFT, RIGHT, CENTER };

public:
	//Represents data of single text input
	struct FontData
	{
		DirectX::XMVECTOR origin{ 0.0, 0.0, 0.0, 0.0 };
		float posX = 0;
		float posY = 0;
		float scale = 0;
		std::string text = "";
		XMVECTOR color = Colors::White;
		TextEngine* textEngineRef;

		void SetText(std::string newString)
		{
			text = newString;
			for (int i = 0; i < text.length(); i++)
			{
				if (text[i] == '.')
					text = text.substr(0, i + 1 + 2);
			}
		}

		int GetIndex()
		{
			return index;
		}
		void SetIndex(int index_)
		{
			index = index_;
		}

	private: int index = 0;
	};

public:
	TextEngine();

	void Initialize(ID3D11Device* d3d, wchar_t const* fontPath);
	FontData* WriteText(ID3D11DeviceContext* deviceContext, float screenWidth, float screenHeight, float posX, float posY, std::string text, float scale = 1.0f, 
		Align align = Align::LEFT, XMVECTOR color = DirectX::Colors::White);
	///<summary>Render all texts collected by TextEngine</summary>
	void RenderText(ID3D11DeviceContext* deviceContext, float screenWidth, float screenHeight);
	///<summary>Fetch single text data entry</summary>
	TextEngine::FontData* GetData(int index);

private:
	std::unique_ptr<DirectX::SpriteFont> m_font;
	std::vector<FontData> m_data;
};
#endif // !_TEXT_ENGINE_H