#include "AnimNode.h"

void AnimNode_Clip::Update(float delta, AnimParam params)
{
	float val = 1.0f;

	auto iter = params.floats.find(name + "_AnimSpeed");

	if (iter != params.floats.end())
	{
		val = *iter->second;
	}

	time += delta * val;
}

void AnimNode_Clip::Evaluate(AnimPose & outPose, Skeleton * skeleton, map<string, AnimationClip*> animClips, AnimParam params)
{
	auto clip = animClips[name];

	float duration = clip->GetDuration();

	
	if (time >= duration)
		time -= duration;

	if (loop)
		time = fmod(time, duration);
	else
		time = min(time, duration);

	clip->GetPoseAtTime(outPose.bones, skeleton, time);
}

void AnimNode_Blend1D::Evaluate(AnimPose & outPose, Skeleton * skeleton, map<string, AnimationClip*> animClips, AnimParam params)
{
	nodeA->Evaluate(poseA, skeleton, animClips, params);
	nodeB->Evaluate(poseB, skeleton, animClips, params);

	int count = poseA.bones.size();

	outPose.bones.resize(count);

	for (int i = 0; i < count; i++)
	{
		outPose.bones[i] = Math::Lerp(poseA.bones[i],poseB.bones[i], alpha);
	}
}

void AnimNode_Blend2D::Evaluate(AnimPose & outPose, Skeleton * skeleton, map<string, AnimationClip*> animClips, AnimParam params)
{
	// 간단히 가장 가까운 애니메이션 선택
	float bestDist = FLT_MAX;
	AnimNode_Clip* best = nullptr;

	for (auto& s : samples)
	{
		float dx = paramX - s.x;
		float dy = paramY - s.y;

		float d = dx * dx + dy * dy;

		if (d < bestDist)
		{
			bestDist = d;
			best = s.node;
		}
	}

	best->Evaluate(outPose, skeleton, animClips, params);
}

void AnimNode_StateMachine::Evaluate(AnimPose & outPose, Skeleton * skeleton, map<string, AnimationClip*> animClips, AnimParam params)
{
	auto& state = states[currentState];

	// Transition 검사
	for (auto& t : state.transitions)
	{
		float animTime = 0.f;
		state.node->GetCurTime(animTime);

		if (t->Checking(params, animTime, animClips.find(currentState)->second->GetDuration()))
		{
			if (prevNode != state.node)
			{
				state.node->SetCurTime(0.0f);
				prevNode = state.node;
			}

			currentState = t->GetTargetState();

			blendTime = t->GetBlendTime();

			transitionTimer = 0.f;
		}
	}

	states[currentState].node->Evaluate(outPose, skeleton, animClips, params);
}

bool AnimNode_LayerBlend::IsChildOfMask(Skeleton* skeleton, int boneIndex)
{
	int parent = boneIndex;

	while (parent != -1)
	{
		if (parent == maskBoneIndex)
			return true;

		parent = skeleton->GetBones()[parent].parentIndex;
	}

	return false;
}

void AnimNode_LayerBlend::Evaluate(AnimPose & outPose, Skeleton * skeleton, map<string, AnimationClip*> animClips, AnimParam params)
{
	AnimPose basePose;
	AnimPose layerPose;

	baseNode->Evaluate(basePose, skeleton, animClips, params);
	layerNode->Evaluate(layerPose, skeleton, animClips, params);

	outPose = basePose;

	size_t boneCount = skeleton->GetBones().size();

	for (UINT i = 0; i < boneCount; i++)
	{
		if (IsChildOfMask(skeleton, i))
		{
			outPose[i] = layerPose[i];
		}
	}
}
