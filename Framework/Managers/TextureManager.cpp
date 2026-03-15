#include "TextureManager.h"

TexturePool::TexturePool(size_t capacity)
{
	pool.resize(capacity);

	for (size_t i = 0; i < capacity; ++i)
		freeList.push_back(&pool[i]);
};

Texture * TexturePool::Allocate(ID3D11ShaderResourceView * srv)
{
	if (freeList.empty())
		return nullptr;

	Texture* mem = freeList.back();
	freeList.pop_back();

	// placement new
	return new((Texture*)mem) Texture(srv);
}

ShadowMap * TexturePool::AllocateShadowMap(UINT width, UINT height)
{
	if (freeList.empty())
		return nullptr;

	Texture* mem = freeList.back();
	freeList.pop_back();

	// placement new
	return new((ShadowMap*)mem) ShadowMap(width, height);
}

void TexturePool::Free(Texture * tex)
{
	tex->~Texture();        
	freeList.push_back(tex);
}

void TextureManager::Initialize()
{
}

void TextureManager::Destroy()
{
	for (auto& srv : srvMap)
	{
		SafeRelease(srv.second.srv);
	}

	srvMap.clear();
}


Texture* TextureManager::LoadTexture(const wstring & imageFile)
{
	// 이미 SRV가 있으면 재사용
	ID3D11ShaderResourceView* srv = nullptr;
	Vector2 spriteSize = Vector2(0.0f, 0.0f);
	Texture* texture = nullptr;

	wstring fullPath = L"../_Assets/_Textures/" + imageFile;

	auto it = srvMap.find(fullPath);

	if (it != srvMap.end())
	{
		srv			= it->second.srv;
		spriteSize	= it->second.spriteSize;
	}
	else
	{
		if (!Path::ExistFile(fullPath))
		{
			wstring str = L"Texture | File missing :  " + fullPath;

			MessageBoxW(NULL, str.c_str(), L"Error", MB_OK);
			return nullptr;
		}

		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
			D3D::GetDevice(),
			fullPath.c_str(),
			nullptr,
			nullptr,
			&srv,
			nullptr
		);
		Check(hr);

		//	// Image size 입력

		// ImageFile 에 대한 info 를 가져온다.
		D3DX11_IMAGE_INFO imageInfo;

		hr = D3DX11GetImageInfoFromFile
		(
			fullPath.c_str(),
			nullptr,
			&imageInfo,
			nullptr
		);

		Check(hr);

		srvMap[fullPath].srv			= srv;
		srvMap[fullPath].spriteSize	= Vector2((float)imageInfo.Width, (float)imageInfo.Height);

		spriteSize = Vector2((float)imageInfo.Width, (float)imageInfo.Height);
	}
	
	texture = texturePool.Allocate(srv);
	texture->SetImageSize(spriteSize);

	// 풀에서 Texture 객체 가져오기
	return texture;
}

Texture * TextureManager::LoadTextureByAbsoultePath(const wstring & imageFile)
{
	ID3D11ShaderResourceView* srv = nullptr;
	Vector2 spriteSize = Vector2(0.0f, 0.0f);
	Texture* texture = nullptr;

	wstring fullPath = imageFile;

	auto it = srvMap.find(fullPath);

	if (it != srvMap.end())
	{
		srv = it->second.srv;
		spriteSize = it->second.spriteSize;
	}
	else
	{
		if (!Path::ExistFile(fullPath))
		{
			wstring str = L"Texture | File missing :  " + fullPath;

			MessageBoxW(NULL, str.c_str(), L"Error", MB_OK);
			return nullptr;
		}

		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
			D3D::GetDevice(),
			fullPath.c_str(),
			nullptr,
			nullptr,
			&srv,
			nullptr
		);
		Check(hr);

		//	// Image size 입력

		// ImageFile 에 대한 info 를 가져온다.
		D3DX11_IMAGE_INFO imageInfo;

		hr = D3DX11GetImageInfoFromFile
		(
			fullPath.c_str(),
			nullptr,
			&imageInfo,
			nullptr
		);

		Check(hr);

		srvMap[fullPath].srv = srv;
		srvMap[fullPath].spriteSize = Vector2((float)imageInfo.Width, (float)imageInfo.Height);

		spriteSize = Vector2((float)imageInfo.Width, (float)imageInfo.Height);
	}

	texture = texturePool.Allocate(srv);
	texture->SetImageSize(spriteSize);

	// 풀에서 Texture 객체 가져오기
	return texture;
}

ShadowMap * TextureManager::CreateShadowMap(UINT width, UINT height)
{
	ShadowMap* shadowMap;
	shadowMap = texturePool.AllocateShadowMap(width, height);

	return shadowMap;
}
