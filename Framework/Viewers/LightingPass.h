#pragma once

#include "Framework.h"

class Shader;
class GBuffer;

class LightingPass
{
public:
	LightingPass();
	~LightingPass();

public:
	void Initialize(UINT width, UINT height);
	void Destroy();

	void Render(GBuffer* gBuffer);

	void ResizeScreen(UINT width, UINT height);

private:
	void CreateFullScreenQuad();
	void CreateSamplerState();
	void CreateDepthState();
	void CreateRenderTarget();

private:
	Shader* shader = nullptr;

	ID3D11Buffer* vertexBuffer	= nullptr;
	ID3D11Buffer* indexBuffer	= nullptr;

	ID3D11Texture2D*			lightTex = nullptr;
	ID3D11RenderTargetView*		lightRTV = nullptr;
	ID3D11ShaderResourceView*	lightSRV = nullptr;

	ID3D11SamplerState* sampler					= nullptr;
	ID3D11DepthStencilState* depthDisableState	= nullptr;

	UINT width = 0;
	UINT height = 0;

};