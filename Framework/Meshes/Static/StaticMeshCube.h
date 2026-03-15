#pragma once

#include "Framework.h"
#include "Base/StaticMeshBase.h"

class GBuffer;

class StaticMeshCube : public StaticMeshBase
{
public:
	StaticMeshCube(float size = 1.0f);
	~StaticMeshCube();

public:
	virtual void Update(Matrix V, Matrix P) override;
	virtual void Render() override;
	virtual void RenderGBuffer(GBuffer* gBuffer);

	virtual void Initialize() override;
	virtual void Destroy() override;

	virtual void SetMaterial(Material* material);

public:
	void UpdateDetails(float size);

private:
	virtual void CreateVertexBuffer()	override;
	virtual void CreateIndexBuffer()	override;

private:
	float size;

};
