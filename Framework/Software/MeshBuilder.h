#pragma once

#include "Framework.h"

#include "StaticMeshBase.h"
#include "Collider/ColliderBase.h"

class ColliderBase;
class StaticMeshBase;
class Material;

class MeshBuilder
{
public:
	MeshBuilder();
	~MeshBuilder();

public:
	static void Clear();
	static void SetClearAfterBuild(bool val) { clearAfterBuild = val; }

public:
	static void BuildStaticMesh(StaticMeshBase* mesh);
	static void BuildStaticMesh(ColliderBase* mesh);
	static void BuildStaticMesh(vector<VertexData>& meshVertices, vector<UINT>& meshIndices);

public:
	static void AddCube(const Vector3& center, const Vector3& scale, const Quaternion& quat = Quaternion(0, 0, 0, 1));
	static void AddCube(const Vector3& center, float size, const Quaternion& quat = Quaternion(0, 0, 0, 1));
	static void AddSphere(const Vector3& center, float radius, int sliceCount, int stackCount, const Quaternion& quat = Quaternion(0, 0, 0, 1));
	static void AddCylinder(const Vector3& center, float radius, float height, int sliceCount, const Quaternion& quat = Quaternion(0, 0, 0, 1));
	static void AddCone(const Vector3& center, float radius, float height, int sliceCount, const Quaternion& quat = Quaternion(0, 0, 0, 1));
	static void AddConeByCosAngle(const Vector3& tip, const Vector3& direction, float height, float cosAngle, int sliceCount);
	static void AddCircle(const Vector3& center, float radius, int sliceCount, const Vector3& normal);


private:
	static vector<VertexData> vertices;
	static vector<UINT> indices;

	static bool clearAfterBuild;

};