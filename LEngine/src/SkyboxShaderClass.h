#pragma once
#ifndef _SKYBOXSHADERCLASS_H_
#define _SKYBOXSHADERCLASS_H_

#include "BaseShaderClass.h"
#include <ScreenGrab.h>

class SkyboxShaderClass : public BaseShaderClass
{
private:
	struct UpVectorBuffer
	{
		XMFLOAT3 upVector;
		float padding;
	};

public:
	//Use different type for normal skybox
	//and for irradiance/specular maps
	enum SkyboxType{
		NORMAL, CONV_DIFFUSE
	};

public:
	ID3D11Resource* m_skyboxTexture;
	ID3D11ShaderResourceView* m_skyboxTextureView;
	virtual bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX&, XMMATRIX&, XMMATRIX&) override;
	virtual bool CreateBufferAdditionals(ID3D11Device * &device) override;

	void SetType(SkyboxType type);
	void SetUpVector(XMFLOAT3 vector);

private:
	SkyboxType m_skyboxType;
	
	ID3D11Buffer* m_upVectorBuffer;
	XMFLOAT3 m_upVector;
};


#endif // !_SKYBOXSHADERCLASS_H_