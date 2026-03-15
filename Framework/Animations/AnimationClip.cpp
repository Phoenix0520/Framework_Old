#include "AnimationClip.h"

AnimationClip::AnimationClip()
{
}

AnimationClip::~AnimationClip()
{
}

KeyFrame AnimationClip::Sample(const BoneAnim& anim, float time)
{
	if (anim.keyFrames.empty())
		return KeyFrame();

	if (anim.keyFrames.size() == 1)
		return anim.keyFrames[0];

	time = fmod(time, duration);

	for (size_t i = 0; i < anim.keyFrames.size() - 1; i++)
	{
		const KeyFrame& a = anim.keyFrames[i];
		const KeyFrame& b = anim.keyFrames[i + 1];

		if (time >= a.time && time <= b.time)
		{
			float t = (time - a.time) / (b.time - a.time);

			KeyFrame result;

			// Translation Lerp
			result.translation = a.translation + (b.translation - a.translation) * t;

			// Scale Lerp
			result.scale =
				a.scale + (b.scale - a.scale) * t;

			Quaternion q1 = a.rotation;
			Quaternion q2 = b.rotation;

			float dot = D3DXQuaternionDot(&q1, &q2);

			if (dot < 0.0f)
			{
				q2.x = -q2.x;
				q2.y = -q2.y;
				q2.z = -q2.z;
				q2.w = -q2.w;
			}

			D3DXQuaternionSlerp(
				&result.rotation,
				&q1,
				&q2,
				t);

			return result;
		}
	}

	return anim.keyFrames.back();
}

void AnimationClip::GetPoseAtTime(vector<Matrix>& outPose, Skeleton* skeleton, float time)
{
	int boneCount = skeleton->GetBones().size();
	outPose.resize(boneCount);

	// 기본은 Bind Local 로 채움
	for (int i = 0; i < boneCount; i++)
	{
		outPose[i] = skeleton->GetBones()[i].localBindPose;
	}

	// 애니메이션이 있는 본만 덮어쓰기
	for (const auto& anim : boneAnimations)
	{
		auto it = skeleton->GetBoneIndexMap().find(anim.boneName);
		if (it == skeleton->GetBoneIndexMap().end())
			continue;

		int index = it->second;

		KeyFrame kf = Sample(anim, time);

		// quaternion 정규화 (중요)
		D3DXQuaternionNormalize(&kf.rotation, &kf.rotation);

		D3DXMATRIX local;
		D3DXMatrixTransformation(
			&local,
			NULL,
			NULL,
			&kf.scale,
			NULL,
			&kf.rotation,
			&kf.translation);

		outPose[index] = local;
	}
}