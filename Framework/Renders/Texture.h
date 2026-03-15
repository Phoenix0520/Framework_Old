#pragma once

#include "Framework.h"

class Texture
{
public:	// 생성자, 소멸자, 구조체
	Texture(ID3D11ShaderResourceView* srv = nullptr);
	Texture(const Texture& otherTexture) {};
	Texture& operator=(const Texture&) {};
	~Texture();
	
public:
	FORCEINLINE void SetSRV(ID3D11ShaderResourceView* srv)	{ this->srv = srv; }
	FORCEINLINE ID3D11ShaderResourceView* GetSRV() const	{ return srv; }

	FORCEINLINE Vector2 GetImageSize() const	{ return imageSize; }
	FORCEINLINE Vector2 GetSpriteOffset() const { return spriteOffset; }
	FORCEINLINE Vector2 GetSpriteSize() const	{ return spriteSize; }

	FORCEINLINE void SetImageSize(Vector2 val)					{ imageSize = val; }
	FORCEINLINE void SetImageSize(float width, float height)	{ imageSize = Vector2(width, height); }
	FORCEINLINE void SetSpriteOffset(Vector2 val)				{ spriteOffset = val; }
	FORCEINLINE void SetSpriteOffset(float x, float y)			{ spriteOffset = Vector2(x, y); }
	FORCEINLINE void SetSpriteSize(Vector2 val)					{ spriteSize = val; }
	FORCEINLINE void SetSpriteSize(float x, float y)			{ spriteSize = Vector2(x, y); }

public:
	FORCEINLINE float GetWidth() { return imageSize.x; }
	FORCEINLINE float GetHeight() { return imageSize.y; }

protected:
	ID3D11ShaderResourceView* srv = nullptr;
	Vector2 imageSize = Vector2(0, 0);

	// 스프라이트 아틀라스용 UV
	Vector2 spriteOffset = Vector2(0, 0);
	Vector2 spriteSize = Vector2(0, 0);
};

class ShadowMap : public Texture
{
public:
	ShadowMap(UINT width, UINT height);
	~ShadowMap();

public:

protected:
	ID3D11Texture2D* tex = nullptr;
	ID3D11DepthStencilView* dsv = nullptr;


};