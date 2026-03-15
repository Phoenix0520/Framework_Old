#include "MeshBuilder.h"

vector<VertexData> MeshBuilder::vertices = vector<VertexData>();
vector<UINT> MeshBuilder::indices = vector<UINT>();
bool MeshBuilder::clearAfterBuild = true;

MeshBuilder::MeshBuilder()
{
}

MeshBuilder::~MeshBuilder()
{
	Clear();
}

void MeshBuilder::Clear()
{
	vertices.clear();
	indices.clear();
}

void MeshBuilder::BuildStaticMesh(StaticMeshBase * mesh)
{
	mesh->Create(vertices, indices);

	if (clearAfterBuild)
		Clear();
}

void MeshBuilder::BuildStaticMesh(ColliderBase * mesh)
{
	mesh->CreateMesh(vertices, indices);

	if (clearAfterBuild)
		Clear();
}

void MeshBuilder::BuildStaticMesh(vector<VertexData>& meshVertices, vector<UINT>& meshIndices)
{
	meshVertices = vertices;
	meshIndices	= indices;

	if (clearAfterBuild)
		Clear();
}

void MeshBuilder::AddCube(const Vector3 & center, const Vector3 & scale, const Quaternion & quat)
{
	float w = scale.x * 0.5f;
	float h = scale.y * 0.5f;
	float d = scale.z * 0.5f;

	vertices.resize(24);

	Vector3 faceNormals[6] =
	{
		Vector3(+0, +0, -1), // Front
		Vector3(+0, +0, +1), // Back
		Vector3(+0, +1, +0), // Top
		Vector3(+0, -1, +0), // Bottom
		Vector3(-1, +0, +0), // Left
		Vector3(+1, +0, +0)  // Right
	};

	Vector3 faceTangents[6] =
	{
		Vector3(+1, +0, +0),
		Vector3(-1, +0, +0),
		Vector3(+1, +0, +0),
		Vector3(+1, +0, +0),
		Vector3(+0, +0, +1),
		Vector3(+0, +0, -1)

	};

	Vector3 pos[24] =
	{
		// Front
		{-w,+h,-d},{+w,+h,-d},{+w,-h,-d},{-w,-h,-d},
		// Back
		{+w,+h,+d},{-w,+h,+d},{-w,-h,+d},{+w,-h,+d},
		// Top
		{-w,+h,+d},{+w,+h,+d},{+w,+h,-d},{-w,+h,-d},
		// Bottom
		{-w,-h,-d},{+w,-h,-d},{+w,-h,+d},{-w,-h,+d},
		// Left
		{-w,+h,+d},{-w,+h,-d},{-w,-h,-d},{-w,-h,+d},
		// Right
		{+w,+h,-d},{+w,+h,+d},{+w,-h,+d},{+w,-h,-d},
	};

	for (int i = 0; i < 24; i++)
	{
		vertices[i].position = Math::RotateVector(quat, pos[i] + center);
		vertices[i].normal = faceNormals[i / 4];
		vertices[i].tangent = faceTangents[i / 4];
	}

	// UV 기본
	for (int i = 0; i < 6; i++)
	{
		int base = i * 4;

		vertices[base + 0].uv = { 0,0 };
		vertices[base + 1].uv = { 1,0 };
		vertices[base + 2].uv = { 1,1 };
		vertices[base + 3].uv = { 0,1 };
	}

	// Index
	UINT index = 0;
	for (UINT i = 0; i < 6; i++)
	{
		indices.push_back(index + 0);
		indices.push_back(index + 1);
		indices.push_back(index + 3);

		indices.push_back(index + 1);
		indices.push_back(index + 2);
		indices.push_back(index + 3);

		index += 4;
	}
}

void MeshBuilder::AddCube(const Vector3 & center, float size, const Quaternion & quat)
{
	AddCube(center, Vector3(size, size, size), quat);
}

void MeshBuilder::AddSphere(const Vector3 & center, float radius, int sliceCount, int stackCount, const Quaternion& quat)
{
	UINT baseVertex = (UINT)vertices.size();

	for (int stack = 0; stack <= stackCount; stack++)
	{
		float v = (float)stack / stackCount;
		float phi = v * Math::PI;

		for (int slice = 0; slice <= sliceCount; slice++)
		{
			float u = (float)slice / sliceCount;
			float theta = u * Math::PI * 2.0f;

			Vector3 dir =
			{
				sinf(phi) * cosf(theta),
				cosf(phi),
				sinf(phi) * sinf(theta)
			};

			VertexData vtx;
			vtx.position = Math::RotateVector(quat, center + dir * radius);
			vtx.normal = dir;
			vtx.tangent = { -sinf(theta), 0, cosf(theta) };
			vtx.uv = { u, v };

			vertices.push_back(vtx);
		}
	}

	UINT ringVertexCount = sliceCount + 1;

	for (int stack = 0; stack < stackCount; stack++)
	{
		for (int slice = 0; slice < sliceCount; slice++)
		{
			UINT a = baseVertex + stack * ringVertexCount + slice;
			UINT b = baseVertex + (stack + 1) * ringVertexCount + slice;
			UINT c = baseVertex + (stack + 1) * ringVertexCount + slice + 1;
			UINT d = baseVertex + stack * ringVertexCount + slice + 1;

			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(d);

			indices.push_back(b);
			indices.push_back(c);
			indices.push_back(d);
		}
	}
}

void MeshBuilder::AddCylinder(const Vector3 & center, float radius, float height, int sliceCount, const Quaternion& quat)
{
	UINT baseVertex = (UINT)vertices.size();

	float halfH = height * 0.5f;

	// ================= SIDE =================
	for (int i = 0; i <= sliceCount; i++)
	{
		float u = (float)i / sliceCount;
		float theta = u * Math::PI * 2.0f;

		float x = cosf(theta);
		float z = sinf(theta);

		Vector3 normal = { x, 0, z };
		Vector3 tangent = { -z, 0, x };

		VertexData vTop;
		VertexData vBottom;

		vTop.position = Math::RotateVector(quat, center + Vector3(x * radius, +halfH, z * radius));
		vBottom.position = Math::RotateVector(quat, center + Vector3(x * radius, -halfH, z * radius));

		vTop.normal = vBottom.normal = normal;
		vTop.tangent = vBottom.tangent = tangent;

		vTop.uv = { u, 0 };
		vBottom.uv = { u, 1 };

		vertices.push_back(vTop);
		vertices.push_back(vBottom);
	}

	for (int i = 0; i < sliceCount; i++)
	{
		UINT a = baseVertex + i * 2;
		UINT b = baseVertex + i * 2 + 1;
		UINT c = baseVertex + i * 2 + 2;
		UINT d = baseVertex + i * 2 + 3;

		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);

		indices.push_back(c);
		indices.push_back(b);
		indices.push_back(d);
	}

	// ================= TOP CAP =================
	UINT topCenter = (UINT)vertices.size();

	VertexData top;
	top.position = Math::RotateVector(quat, center + Vector3(0, halfH, 0));
	top.normal = { 0,1,0 };
	top.tangent = { 1,0,0 };
	top.uv = { 0.5f,0.5f };

	vertices.push_back(top);

	for (int i = 0; i <= sliceCount; i++)
	{
		float u = (float)i / sliceCount;
		float theta = u * Math::PI * 2.0f;

		float x = cosf(theta);
		float z = sinf(theta);

		VertexData v;
		v.position = Math::RotateVector(quat, center + Vector3(x * radius, halfH, z * radius));
		v.normal = { 0,1,0 };
		v.tangent = { 1,0,0 };
		v.uv = { x * 0.5f + 0.5f, z * 0.5f + 0.5f };

		vertices.push_back(v);
	}

	for (int i = 0; i < sliceCount; i++)
	{
		indices.push_back(topCenter);
		indices.push_back(topCenter + i + 1);
		indices.push_back(topCenter + i + 2);
	}

	// ================= BOTTOM CAP =================
	UINT bottomCenter = (UINT)vertices.size();

	VertexData bottom;
	bottom.position = Math::RotateVector(quat, center + Vector3(0, -halfH, 0));
	bottom.normal = { 0,-1,0 };
	bottom.tangent = { 1,0,0 };
	bottom.uv = { 0.5f,0.5f };

	vertices.push_back(bottom);

	for (int i = 0; i <= sliceCount; i++)
	{
		float u = (float)i / sliceCount;
		float theta = u * Math::PI * 2.0f;

		float x = cosf(theta);
		float z = sinf(theta);

		VertexData v;
		v.position = center + Vector3(x * radius, -halfH, z * radius);
		v.normal = { 0,-1,0 };
		v.tangent = { 1,0,0 };
		v.uv = { x * 0.5f + 0.5f, z * 0.5f + 0.5f };

		vertices.push_back(v);
	}

	for (int i = 0; i < sliceCount; i++)
	{
		indices.push_back(bottomCenter);
		indices.push_back(bottomCenter + i + 2);
		indices.push_back(bottomCenter + i + 1);
	}
}

void MeshBuilder::AddCone(const Vector3 & center, float radius, float height, int sliceCount, const Quaternion& quat)
{
	UINT baseVertex = (UINT)vertices.size();

	float halfH = height * 0.5f;

	Vector3 tip = Math::RotateVector(quat, center + Vector3(0, halfH, 0));

	// ================= SIDE =================
	for (int i = 0; i <= sliceCount; i++)
	{
		float u = (float)i / sliceCount;
		float theta = u * Math::PI * 2.0f;

		float x = cosf(theta);
		float z = sinf(theta);

		Vector3 normal = { x, radius / height, z };

		D3DXVec3Normalize(&normal, &normal);

		VertexData v;
		v.position = Math::RotateVector(quat, center + Vector3(x * radius, -halfH, z * radius));
		v.normal = normal;
		v.tangent = { -z, 0, x };
		v.uv = { u, 1 };

		vertices.push_back(v);
	}

	UINT tipIndex = (UINT)vertices.size();

	VertexData tipV;
	tipV.position = Math::RotateVector(quat, tip);
	tipV.normal = { 0,1,0 };
	tipV.tangent = { 1,0,0 };
	tipV.uv = { 0.5f,0 };

	vertices.push_back(tipV);

	for (int i = 0; i < sliceCount; i++)
	{
		indices.push_back(baseVertex + i);
		indices.push_back(baseVertex + i + 1);
		indices.push_back(tipIndex);
	}

	// ================= BOTTOM =================
	UINT bottomCenter = (UINT)vertices.size();

	VertexData bottom;
	bottom.position = Math::RotateVector(quat, center + Vector3(0, -halfH, 0));
	bottom.normal = { 0,-1,0 };
	bottom.tangent = { 1,0,0 };
	bottom.uv = { 0.5f,0.5f };

	vertices.push_back(bottom);

	for (int i = 0; i <= sliceCount; i++)
	{
		float u = (float)i / sliceCount;
		float theta = u * Math::PI * 2.0f;

		float x = cosf(theta);
		float z = sinf(theta);

		VertexData v;
		v.position = Math::RotateVector(quat, center + Vector3(x * radius, -halfH, z * radius));
		v.normal = { 0,-1,0 };
		v.tangent = { 1,0,0 };
		v.uv = { x * 0.5f + 0.5f, z * 0.5f + 0.5f };

		vertices.push_back(v);
	}

	for (int i = 0; i < sliceCount; i++)
	{
		indices.push_back(bottomCenter);
		indices.push_back(bottomCenter + i + 2);
		indices.push_back(bottomCenter + i + 1);
	}
}

void MeshBuilder::AddConeByCosAngle(const Vector3& tip, const Vector3& direction, float height, float cosAngle, int sliceCount)
{
	sliceCount = (sliceCount % 2 == 0) ? sliceCount : sliceCount + 1;
	int half = sliceCount / 2;

	UINT baseVertex = (UINT)vertices.size();

	float angle = acosf(cosAngle);
	float bottomRadius = height * tanf(angle);

	Vector3 up = direction;
	Vector3 right;
	D3DXVec3Cross(&right, &up, &Vector3(0, 1, 0));
	if (D3DXVec3Length(&right) < 0.001f)
		D3DXVec3Cross(&right, &up, &Vector3(1, 0, 0));
	D3DXVec3Normalize(&right, &right);

	Vector3 forward;
	D3DXVec3Cross(&forward, &right, &up);
	Vector3 baseCenter = tip + up * height;

	// ====== 0~180도 ======
	for (int i = 0; i <= half; i++)
	{
		float t = (float)i / half;       // 0~1
		float theta = t * Math::PI;      // 0~π

		float x = cosf(theta);
		float z = sinf(theta);

		Vector3 circlePos =
			right * (x * bottomRadius) +
			forward * (z * bottomRadius);

		VertexData v;
		v.position = baseCenter + circlePos;

		Vector3 normal = circlePos - up * bottomRadius;
		D3DXVec3Normalize(&normal, &normal);
		v.normal = normal;

		v.tangent = right;
		v.uv = { t, 1.0f };              // 0~1

		vertices.push_back(v);
	}

	// ====== 180~360도 ======
	UINT secondStart = (UINT)vertices.size();

	for (int i = 0; i <= half; i++)
	{
		float t = (float)i / half;           // 0~1
		float theta = Math::PI + t * Math::PI;  // π~2π

		float x = cosf(theta);
		float z = sinf(theta);

		Vector3 circlePos =
			right * (x * bottomRadius) +
			forward * (z * bottomRadius);

		VertexData v;
		v.position = baseCenter + circlePos;

		Vector3 normal = circlePos - up * bottomRadius;
		D3DXVec3Normalize(&normal, &normal);
		v.normal = normal;

		v.tangent = right;
		v.uv = { t, 1.0f };                  // 다시 0~1

		vertices.push_back(v);
	}

	// ===== Tip =====
	UINT tipIndex = (UINT)vertices.size();

	VertexData tipV;
	tipV.position = tip;
	tipV.normal = -up;
	tipV.tangent = right;
	tipV.uv = { 0.5f, 0.0f };

	vertices.push_back(tipV);

	// ===== 인덱스 =====

	// 첫 반원
	for (int i = 0; i < half; i++)
	{
		indices.push_back(baseVertex + i);
		indices.push_back(baseVertex + i + 1);
		indices.push_back(tipIndex);
	}

	// 두 번째 반원
	for (int i = 0; i < half; i++)
	{
		indices.push_back(secondStart + i);
		indices.push_back(secondStart + i + 1);
		indices.push_back(tipIndex);
	}
}

void MeshBuilder::AddCircle(const Vector3 & center, float radius, int slice, const Vector3 & normal)
{
}
