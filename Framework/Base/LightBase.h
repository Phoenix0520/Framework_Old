#pragma once

#include "Framework.h"

#define LIGHTTYPE_DIRECTIONAL	0
#define LIGHTTYPE_POINT			1
#define LIGHTTYPE_SPOT			2


struct LightDesc;
struct LightingBuffer;
class Material;

class LightBase
{
public:
	LightBase();
	~LightBase();

public:
	virtual void Initialize();
	virtual void Destroy();

	virtual void Update(Matrix V, Matrix P);
	virtual void Render();

	void UpdateDetails();

public:
	FORCEINLINE LightDesc GetLightingBuffer() { return lightDesc; }
	FORCEINLINE void SetLightingBuffer(LightDesc desc) { lightDesc = desc; }

	FORCEINLINE Transform* GetTransform() { return transform; }
		 
public:

	FORCEINLINE void SetLightPosition(Vector3 pos) { lightDesc.lightPos = pos; };
	FORCEINLINE void SetLightDirection(Vector3 dir) { lightDesc.lightDir = dir; };
	FORCEINLINE void SetLightColor(Vector3 color) { lightDesc.lightColor = color; };
	FORCEINLINE void SetLightColor(Color color) { lightDesc.lightColor = Vector3(color.r, color.g, color.b); };
	FORCEINLINE void SetLightIntensity(float val) { lightDesc.lightIntensity = val; };
	FORCEINLINE void SetLightRadius(float radius) { lightDesc.lightRadius = radius; };
	
	FORCEINLINE void SetLightInnerCone(float innerCone) { lightDesc.innerCone = innerCone; }
	FORCEINLINE void SetLightOuterCone(float outerCone) { lightDesc.outerCone = outerCone; }

protected:
	virtual void CreateVertexBuffer();
	virtual void CreateIndexBuffer();

	void DeleteBuffer();

protected:
	Transform* transform;

	LightDesc lightDesc;

	Material* material;

	// Debug Mesh
	vector<VertexData>		vertices;
	vector<UINT>			indices;
	ID3D11Buffer*			vertexBuffer = nullptr;
	ID3D11Buffer*			indexBuffer = nullptr;
	UINT					vertexCount = 0;
	UINT					indexCount = 0;

	bool isActive	= true;
	bool isVisible  = true;
};