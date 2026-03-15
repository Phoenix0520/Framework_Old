#pragma once

#include "Framework.h"

#include "Meshes/Skeletal/Skeleton.h"

class Skeleton;

enum ResourceType
{
	MIXAMO,
	BLENDER,
	MAYA,
	MAX
};

class SkeletalMeshResource
{
public:
	SkeletalMeshResource();
	~SkeletalMeshResource();

	bool LoadBinary(const string& path);

	Skeleton* GetSkeleton() { return skeleton; }

	ResourceType GetResourceType() { return resType; }
	void SetResourceType(ResourceType val) { resType = val; }

private:
	Skeleton* skeleton = nullptr;
	ResourceType resType = MIXAMO;

};