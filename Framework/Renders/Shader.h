#pragma once

#include "Framework.h"

class Shader
{
public:
	Shader() = default;
	Shader(const Shader& otherShader) {}
	Shader& operator= (const Shader&) {}
	~Shader();

public:
	bool Initialize(const wstring& shaderFile, bool skeletal);
	void Bind();

public:
	void SetSRV(vector<ID3D11ShaderResourceView*> srvs);

	FORCEINLINE ID3D11InputLayout* GetLayout() const { return inputLayout; }
	
private:
	void CreateInputLayout();
	void CreateInputLayoutForSkeletalMesh();

private:
	wstring shaderFileName;

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D10Blob*			vsBlob = nullptr;
	ID3D11PixelShader*	pixelShader = nullptr;
	ID3D10Blob*			psBlob = nullptr;
	ID3D11InputLayout*	inputLayout = nullptr;
	// Com Interface

	bool forSkeletal = false;

};