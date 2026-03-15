#pragma once

#include "Framework.h"
#include "Base/StaticMeshBase.h"

class StaticMeshSphere : public StaticMeshBase
{
public:
	StaticMeshSphere(float radius = 1.0f, UINT radialSlice = 16, UINT radialStack = 16);
	~StaticMeshSphere();

public:
	virtual void Initialize() override;
	virtual void Destroy() override;

	virtual void Update(Matrix V, Matrix P) override;
	virtual void Render() override;

	virtual void SetMaterial(Material* material) override;

	void UpdateDetails(float radius, UINT radialSlice, UINT radialStack);

public:
	virtual void CreateVertexBuffer() override;
	virtual void CreateIndexBuffer() override;

private:
	float radius;
	UINT radialSlice;
	UINT radialStack;

};