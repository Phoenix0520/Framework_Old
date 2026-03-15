#pragma once

#include "Framework.h"
#include "Base/Transform.h"

#define HOVERHIGHLIGHT true

class Material;
class Transform;

class ColliderBase
{
public:
	ColliderBase();
	~ColliderBase();

public:
	virtual void Update(Matrix V, Matrix P);
	virtual void Render();

	virtual void Initialize();
	virtual void Destroy();

	void CreateMesh(vector<VertexData> vertices, vector<UINT> indices);

	// 동작
public:
	virtual void GetAABB(Vector3& outMin, Vector3& outMax) = 0;

	virtual bool Intersect(ColliderBase* other);
	virtual bool IntersectMouse(float& outDistance);

	// 충돌 연산

	FORCEINLINE Transform* GetTransform() { return transform; }
	FORCEINLINE void SetTransform(Transform* val) { transform = val; }

public:
	virtual void SetMaterial(Material* material);
	FORCEINLINE Material* GetMaterial() { return material; }


public:
	FORCEINLINE bool IsHoverd()					{ return isHovered; }
	FORCEINLINE void SetHoverState(bool state)	{ isHovered = state; }

	FORCEINLINE bool IsVisible()				{ return isVisible; }
	FORCEINLINE void SetVisibleState(bool val)	{ isVisible = val; }

protected:
	virtual void CreateVertexBuffer();
	virtual void CreateIndexBuffer();

	void DeleteBuffers();

protected:
	Transform* transform = nullptr;
	
	Material* material = nullptr;

	vector<VertexData>		vertices;
	vector<UINT>			indices;
	ID3D11Buffer*			vertexBuffer = nullptr;
	ID3D11Buffer*			indexBuffer = nullptr;
	UINT					vertexCount = 0;
	UINT					indexCount = 0;

	bool isHovered	= false;
	bool isVisible	= true;

};