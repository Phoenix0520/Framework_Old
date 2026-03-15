#pragma once

#include "Framework.h"
#include "Base/StaticMeshBase.h"

class StaticMeshCylinder : public StaticMeshBase
{
public:
	StaticMeshCylinder(float radius = 1.0f, float height = 1.0f, UINT radialSlice = 16);
	~StaticMeshCylinder();

public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	
	virtual void Update(Matrix V, Matrix P) override;
	virtual void Render() override;

	virtual void SetMaterial(Material* material) override;

	void UpdateDetails(float radius, float height, UINT radialSlice);

public:
	virtual void CreateVertexBuffer() override;
	virtual void CreateIndexBuffer() override;

private:
	float radius;
	float height;
	UINT radialSlice;

};