#pragma once

#include "Framework.h"

#include "Base/Transform.h"
#include "Meshes/Static/StaticMeshSphere.h"
#include "Collider/BoxCollider.h"

class StaticMeshSphere;
class BoxCollider;

class Ezreal_A : public GameObject
{
public:
	Ezreal_A();
	~Ezreal_A(); 

public:
	virtual void Update(Matrix V, Matrix P);
	virtual void Render();
	virtual void RenderGBuffer(GBuffer* gBuffer);

	void Shoot();

public:
	FORCEINLINE StaticMeshSphere* GetMesh() { return staticMesh; }
	FORCEINLINE BoxCollider*	GetCollider() { return collider; }

private:
	StaticMeshSphere* staticMesh;
	BoxCollider* collider;

	bool shooting = false;

};