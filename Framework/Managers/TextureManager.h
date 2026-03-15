#pragma once
#include "Framework.h"

#include "Base/Singleton.h"
#include "Renders/Texture.h"

struct SpriteResource
{
public:
	ID3D11ShaderResourceView* srv;
	Vector2 spriteSize;
};

class Texture;
class ShadowMap;

class TexturePool
{
public:
	TexturePool(size_t capacity);

public:
	Texture* Allocate(ID3D11ShaderResourceView* srv);
	ShadowMap* AllocateShadowMap(UINT width, UINT height);
	void Free(Texture* tex);

private:
	vector<Texture> pool;
	vector<Texture*> freeList;
};

class TextureManager : public Singleton<TextureManager>
{
private:
	TextureManager() {}
	TextureManager(const TextureManager& other) {}
	TextureManager(const TextureManager* other) {}
	~TextureManager() {}

public: 
	friend Singleton;

	virtual void Initialize() override;
	virtual void Destroy() override;

	virtual void Update() override NoDefine
	virtual void Render() override NoDefine

	Texture* LoadTexture(const wstring& imageFile);
	Texture* LoadTextureByAbsoultePath(const wstring& imageFile);

	ShadowMap* CreateShadowMap(UINT width, UINT height);


private:
	unordered_map<wstring, SpriteResource> srvMap;

	TexturePool texturePool { 1024 };

};