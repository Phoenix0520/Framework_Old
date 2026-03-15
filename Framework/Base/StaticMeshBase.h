#pragma once

#include "Framework.h"
#include "Base/Transform.h"

class Texture;
class Transform;
class Material;
class GBuffer;

class StaticMeshBase
{
protected:
	StaticMeshBase();
	virtual ~StaticMeshBase();

public:
	virtual void Initialize();
	virtual void Destroy();

	virtual void Update(Matrix V, Matrix P);
	virtual void Render();
	virtual void RenderGBuffer(GBuffer* gBuffer);

	virtual void Create(vector<VertexData> vertices, vector<UINT> indices);

public:
	virtual void SetMaterial(Material* material);
	FORCEINLINE Material* GetMaterial() { return material; }

public:
	FORCEINLINE Transform*		GetTransform()													{ return transform; }

	FORCEINLINE void			SetTransform(Transform* val)									{ transform = val; }
	//FORCEINLINE void			SetTransform(Vector3 position, Vector3 rotation, Vector3 scale) { transform = new Transform(position, rotation, scale); }
	// 트랜스폼

	FORCEINLINE bool			IsVisible()					{ return isVisible; }
	FORCEINLINE void			SetVisibleState(bool val)	{ isVisible = val; }

protected:
	virtual void CreateVertexBuffer();
	virtual void CreateIndexBuffer();
	virtual void UpdateVertexBuffer();
	virtual void UpdateIndexBuffer();
	
	void DeleteBuffers();

protected:
	Transform*	transform = nullptr;
	// 트랜스폼

	Material* material = nullptr;

	vector<VertexData>	vertices;
	vector<UINT>			indices;
	ID3D11Buffer*			vertexBuffer	= nullptr;
	ID3D11Buffer*			indexBuffer		= nullptr;
	UINT					vertexCount		= 0;
	UINT					indexCount		= 0;
	// 정점 정보

	bool					isVisible		= true;

};