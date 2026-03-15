#pragma once
#include "Framework.h"
#include "ColliderBase.h"


class BoxCollider : public ColliderBase
{
public:
	BoxCollider();
	~BoxCollider();

public:
	virtual void Update(Matrix V, Matrix P) final override;
	virtual void Render() final override;

	virtual void Initialize();
	virtual void Destroy();

public:
	virtual void GetAABB(Vector3& outMin, Vector3& outMax);


public:
	virtual void SetMaterial(Material* material) final override;

private:
	virtual void CreateVertexBuffer()	final override;
	virtual void CreateIndexBuffer()	final override;

private:


};