#pragma once
#include "Framework.h"

#include "Base/Singleton.h"
#include "Renders/Shader.h"

class Shader;

class ShaderPool
{
public:
	ShaderPool(size_t capacity);
	
public:
	Shader* Allocate();
	void Free(Shader* shader);
	

private:
	vector<Shader> pool;
	vector<Shader*> freeList;
};

class ShaderManager : public Singleton<ShaderManager>
{
private:
	ShaderManager();
	~ShaderManager();

public:
	friend Singleton;

	virtual void Initialize() override;
	virtual void Destroy() override;

	Shader* LoadShader(const wstring& shaderFile, bool skeletal = false);

private:
	unordered_map<wstring, Shader*> shaderMap;
	ShaderPool shaderPool{ 1024 };
};
