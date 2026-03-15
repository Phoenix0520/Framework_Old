#pragma once

#include "Framework.h"

#include "Meshes/Skeletal/Skeleton.h"

class Skeleton;

struct KeyFrame
{
	float time;

	Vector3 translation;
	Vector3 scale;
	Quaternion rotation;
};

struct BoneAnim
{
	int boneIndex;
	string boneName;
	vector<KeyFrame> keyFrames;
};

class AnimationClip
{
public:
	AnimationClip();
	~AnimationClip();

public:
	FORCEINLINE string GetName() { return name; }
	FORCEINLINE float GetDuration() { return duration; }
	FORCEINLINE vector<BoneAnim>& GetBoneAnim() { return boneAnimations; }

	FORCEINLINE void SetName(string str) { name = str; }
	FORCEINLINE void SetDuration(float val) { duration = val; }
	FORCEINLINE void SetBoneAnim(vector<BoneAnim>& boneAnims) { boneAnimations = boneAnims; }
	FORCEINLINE void AddBoneAnim(const BoneAnim& boneAnim) { boneAnimations.push_back(boneAnim); }

public:
	void GetPoseAtTime(vector<Matrix>& outPose, Skeleton* skeleton, float duration);
	KeyFrame Sample(const BoneAnim& anim, float time);

private:
	string name;
	float duration;

	vector<BoneAnim> boneAnimations;

};