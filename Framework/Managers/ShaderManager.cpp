#include "ShaderManager.h"

ShaderPool::ShaderPool(size_t capacity)
{
	pool.resize(capacity);
	for (size_t i = 0; i < capacity; ++i)
		freeList.push_back(&pool[i]);
}

Shader * ShaderPool::Allocate()
{
	if (freeList.empty())
		return nullptr;

	Shader* mem = freeList.back();
	freeList.pop_back();

	return new(mem) Shader(); // placement new
}

void ShaderPool::Free(Shader * shader)
{
	shader->~Shader();
	freeList.push_back(shader);
}

Shader* ShaderManager::LoadShader(const wstring& shaderFile, bool skeletal)
{
	// ÀÌ¹Ì ·ÎµåµÊ
	if (shaderMap.count(shaderFile))
		return shaderMap[shaderFile];

	Shader* shader = shaderPool.Allocate();
	shader->Initialize(shaderFile, skeletal);

	shaderMap.insert({ shaderFile, shader });
	return shader;
}


ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::Initialize()
{
}

void ShaderManager::Destroy()
{
	for (auto& pair : shaderMap)
		shaderPool.Free(pair.second);

	shaderMap.clear();
}
