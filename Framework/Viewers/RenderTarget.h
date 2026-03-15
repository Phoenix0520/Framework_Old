#pragma once

#include "Framework.h"

class DepthStencil;

class RenderTarget
{
public:
	RenderTarget(UINT width = 0, UINT height = 0, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	~RenderTarget();

public:
	FORCEINLINE ID3D11RenderTargetView*		GetRTV() { return rtv; }
	FORCEINLINE ID3D11ShaderResourceView*	GetSRV() { return srv; }

	void SaveTexture(wstring saveFile);
	void PreRender(DepthStencil* dsv);	// 화면 출력전 화면을 지우는 함수

public:
	static void PreRender(RenderTarget** rtvArr, UINT count, DepthStencil* dsv); // 여러 rtv의 화면을 지우는 함수

private:
	UINT width	= 0;
	UINT height = 0;

	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	ID3D11Texture2D*			texture	= nullptr;
	ID3D11RenderTargetView*		rtv		= nullptr;
	ID3D11ShaderResourceView*	srv		= nullptr;
};