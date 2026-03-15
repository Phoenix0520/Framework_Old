#include "SkeletalMeshResource.h"

SkeletalMeshResource::SkeletalMeshResource()
{
	skeleton = new Skeleton();
}

SkeletalMeshResource::~SkeletalMeshResource()
{
	SAFE_DELETE(skeleton);
}

bool SkeletalMeshResource::LoadBinary(const string & path)
{
	ifstream in(path, ios::binary);

	if (!in.is_open())
		return false;

	vector<SkinnedVertex> vertices;
	vector<UINT> indices;
	skeleton->Clear();

	UINT vertexCount = 0;
	in.read((char*)&vertexCount, sizeof(UINT));

	vertices.resize(vertexCount);
	in.read((char*)vertices.data(), sizeof(SkinnedVertex) * vertexCount);

	UINT indexCount = 0;
	in.read((char*)&indexCount, sizeof(UINT));

	indices.resize(indexCount);
	in.read((char*)indices.data(), sizeof(UINT) * indexCount);

	skeleton->SetVertices(vertices);
	skeleton->SetIndices(indices);

	UINT boneCount = 0;
	in.read((char*)&boneCount, sizeof(UINT));

	for (UINT i = 0; i < boneCount; i++)
	{
		UINT nameLen = 0;
		in.read((char*)&nameLen, sizeof(UINT));

		string name;
		name.resize(nameLen);
		in.read(&name[0], nameLen);

		int parentIndex;
		D3DXMATRIX localBind;
		D3DXMATRIX globalBind;
		D3DXMATRIX offset;

		in.read((char*)&parentIndex, sizeof(int));
		in.read((char*)&localBind, sizeof(D3DXMATRIX));
		in.read((char*)&globalBind, sizeof(D3DXMATRIX));
		in.read((char*)&offset, sizeof(D3DXMATRIX));

		BoneData bone = {};
		bone.name			= name;
		bone.parentIndex	= parentIndex;
		bone.localBindPose	= localBind;
		bone.globalBindPose = globalBind;
		bone.offset			= offset;

		skeleton->AddBone(bone);
	}

	in.close();
	return true;
}