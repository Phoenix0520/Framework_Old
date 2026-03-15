#pragma once

#include "Framework.h"
#include "AnimDefs.h"
#include "AnimationClip.h"
#include "AnimNode.h"
#include "Meshes/Skeletal/Skeleton.h"

class Skeleton;
class AnimationClip;


class AnimCondition
{
public:
	AnimCondition() {}
	~AnimCondition() {}

public:
	void SetThreshold(float val) { threshold = val; }
	void SetParamName(string name) { paramName = name; }
	void SetType(AnimConditionType val) { type = val; }
	bool Evaluate(AnimParam& params);

private:
	AnimConditionType type;
	string paramName;

	float threshold = 0.0f;

};

enum TransitionType
{
	PARAM,
	ANIM_END,
	TRIGGERTIME
};

class AnimTransition
{
public:
	bool Checking(AnimParam& params, float curTime, float animDuration);
	
public:
	FORCEINLINE TransitionType GetType() { return type; }
	FORCEINLINE string GetTargetState() { return targetState; }
	FORCEINLINE float GetBlendTime() { return blendTime; }
	FORCEINLINE float GetTriggerTime() { return triggerTime; }

	FORCEINLINE void SetType(TransitionType val) { type = val; }
	FORCEINLINE void SetTargetState(string targetName) { targetState = targetName; }
	FORCEINLINE void SetBlendTime(float val) { blendTime = val; }
	FORCEINLINE void SetTriggerTime(float val) { triggerTime = val; }

	FORCEINLINE map<string, AnimCondition*>& GetConditions() { return conditions; }
	FORCEINLINE void AddConditions(string name, AnimCondition* condition) { conditions.insert(make_pair(name, condition)); }

protected:
	TransitionType type = PARAM;

	string targetState = "IDLE";

	float blendTime = 0.2f;
	float triggerTime = 0.0f;

	map<string, AnimCondition*> conditions;
};

class AnimNode;
class AnimNode_Clip;
class AnimNode_Blend1D;
class AnimNode_Blend2D;
class AnimNode_StateMachine;

class AnimGraph
{
public:
	friend class AnimNode;

public:
	AnimPose& Update(Skeleton* skeleton, map<string, AnimationClip*>& animClips, AnimParam params, float delta = DELTA);
	void SetRootNode(AnimNode* node) { rootNode = node; }

public:
	static AnimNode_Clip*			CreateClip(string name);
	static AnimNode_Blend1D*		CreateBlend1D(AnimNode* n1, AnimNode* n2);
	static AnimNode_StateMachine*	CreateStateMachine();

private:
	AnimNode* rootNode;
	AnimPose finalPose;

};