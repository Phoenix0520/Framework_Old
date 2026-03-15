#pragma once
#include "Framework.h"
#include "Base/Transform.h"
#include "Meshes/Static/StaticMeshCube.h"
#include "Meshes/Static/StaticMeshCylinder.h"
#include "Meshes/Static/StaticMeshSphere.h"
#include "Collider/BoxCollider.h"
#include "GameObject.h"

class StaticMeshSphere;
class StaticMeshCylinder;
class StaticMeshCube;
class BoxCollider;
class GBuffer;

class Block : public GameObject
{
public:
	Block();
	~Block();

public:
	virtual void Update(Matrix V, Matrix P);
	virtual void Render();
	virtual void RenderGBuffer(GBuffer* gBuffer);

public:
	FORCEINLINE StaticMeshCube* GetMesh()		{ return staticMesh; }
	FORCEINLINE BoxCollider*	GetCollider()	{ return collider; }

private:
	StaticMeshCube* staticMesh;
	StaticMeshCylinder* cylinder;
	StaticMeshSphere* sphere;
	BoxCollider* collider;


};