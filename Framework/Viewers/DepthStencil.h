#pragma once

#include "Framework.h"

class DepthStencil
{
public:
	DepthStencil(UINT width = 0, UINT height = 0, bool stnecil = false);
	~DepthStencil();

public:
	FORCEINLINE ID3D11DepthStencilView*		GetDSV() { return dsv; }
	FORCEINLINE ID3D11ShaderResourceView*	GetSRV() { return srv; }

	void SaveTexture(wstring saveFile);

private:
	bool useStencil = false;
	UINT width		= 0;
	UINT height		= 0;

	ID3D11Texture2D*			texture	= nullptr;
	ID3D11DepthStencilView*		dsv		= nullptr;
	ID3D11ShaderResourceView*	srv		= nullptr;
	
};