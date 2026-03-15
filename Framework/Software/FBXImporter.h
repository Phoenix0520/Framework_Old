#pragma once

#include <Assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Framework.h"

#include "Animations/AnimationClip.h"
#include "Meshes/Skeletal/SkeletalMesh.h"

class SkeletalMeshResource;
class AnimationClipResource;

class FBXImporter
{
public:
	static bool LoadFBXFile(const string& path, UINT plag = aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

	static bool SaveMeshBinary(const string& path, const vector<SkinnedVertex>& vertices, const vector<UINT>& indices, Skeleton* skeleton);
	static bool SaveAnimationBinary(const string& path, float duration, AnimationClip* clip);

public:
	static const vector<SkinnedVertex>& GetVertices() { return vertices; }
	static const vector<UINT>& GetIndices() { return indices; }
	static const Skeleton* GetSkeleton() { return skeleton; }
	static const vector<AnimationClip*>& GetAnimations() { return animClip; }

private:
	static void LoadAnimation(const aiScene* scene);
	static void ProcessNode(aiNode* node, const aiScene* scene, int parentIndex);
	static void ProcessMesh(aiMesh* mesh, const aiScene* scene, int parentIndex);
	static void ProcessBones(aiMesh* mesh, const aiScene* scene, int parentIndex);
	static aiNode* FindNode(aiNode* node, const string& name);

private:
	static vector<SkinnedVertex> vertices;
	static vector<UINT> indices;
	static Skeleton* skeleton;
	static vector<AnimationClip*> animClip;

};