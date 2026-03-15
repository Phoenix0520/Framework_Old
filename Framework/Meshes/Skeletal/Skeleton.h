#pragma once

#include "Framework.h"

struct BoneData
{
	string name;

	int parentIndex;		// 부모 본 인덱스 (-1 경우 루트 본)

	Matrix offset;			// BindPose 역행렬
	Matrix localBindPose;	// 초기 로컬 행렬
	Matrix globalBindPose;  // 초기 글로벌 행렬
};

class Skeleton
{
public:
	void Clear()
	{
		bones.clear();
		boneIndexMap.clear();
	}

	int AddBone(const BoneData& bone)
	{
		int index = (int)bones.size();
		bones.push_back(bone);
		boneIndexMap[bone.name] = index;
		return index;
	}

	int GetBoneIndex(const string& name) const
	{
		auto it = boneIndexMap.find(name);
		if (it == boneIndexMap.end())
			return -1;
		return it->second;
	}

	const auto& GetBoneIndexMap() { return boneIndexMap; }
	const vector<BoneData>& GetBones() const { return bones; }

	FORCEINLINE void SetVertices(vector<SkinnedVertex>& vertices) { this->vertices = vertices; }
	FORCEINLINE void SetIndices(vector<UINT>& indices) { this->indices = indices; }
	FORCEINLINE vector<SkinnedVertex>& GetVertices() { return vertices; }
	FORCEINLINE vector<UINT>& GetIndices() { return indices; }


	FORCEINLINE void SetBoneOffset(UINT index, Matrix mat) { bones[index].offset = mat; }
	FORCEINLINE void SetBoneLocalBindPose(UINT index, Matrix mat) { bones[index].localBindPose = mat; }
	FORCEINLINE void SetBoneGlobalBindPose(UINT index, Matrix mat) { bones[index].globalBindPose = mat; }
	FORCEINLINE void SetBoneParentIndex(UINT index, int parentIndex) { bones[index].parentIndex = parentIndex; }

private:
	vector<SkinnedVertex> vertices;
	vector<UINT> indices;

	vector<BoneData> bones;
	unordered_map<string, int> boneIndexMap;

};