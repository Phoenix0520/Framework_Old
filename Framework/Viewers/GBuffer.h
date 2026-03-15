#pragma once

#include "Framework.h"

class Shader;

class GBuffer
{
public:
	GBuffer();
	~GBuffer();

public:
	void Initialize(UINT width, UINT height);
	void Destroy();

public:
	void SetRenderTargets();
	void Clear();

	void Bind();
	void BindSRVS(UINT startSlot = 0);
	void UnbindSRVS();
	
	void BindShader();

	void ResizeScreen(UINT width, UINT height);

	ID3D11DepthStencilView*		GetDSV()			{ return depthStencilView; }
	ID3D11RenderTargetView*		GetRTV(UINT index)	{ return rtvs[index]; }
	ID3D11ShaderResourceView*	GetSRV(UINT index)	{ return srvs[index]; }
	ID3D11ShaderResourceView*	GetDepthSRV()		{ return depthSrv; }

	Shader* GetShader() { return shader; }

private:
	ID3D11Texture2D*			textures[3];
	ID3D11RenderTargetView*		rtvs[3];
	ID3D11ShaderResourceView*	srvs[3];

	ID3D11Texture2D*			depthTex;
	ID3D11DepthStencilView*		depthStencilView;
	ID3D11ShaderResourceView*   depthSrv;

	Shader* shader = nullptr;
};