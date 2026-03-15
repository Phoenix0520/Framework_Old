#pragma once

#include "Framework.h"
#include "Meshes/Skeletal/Skeleton.h"
#include "AnimDefs.h"
#include "AnimationClip.h"
#include "AnimGraph.h"

class AnimGraph;
class AnimTransition;

class AnimNode
{
public:
	AnimNode() {} ;
	virtual ~AnimNode() {};

public:
	virtual void Update(float delta, AnimParam params) = 0;
	virtual void Evaluate(AnimPose& outPose, Skeleton* skeleton, map<string, AnimationClip*> animClips, AnimParam params) = 0;
	virtual void GetCurTime(float& val) {}
	virtual void SetCurTime(float val) {}
public:
	friend class AnimGraph;

protected:
	

};

class AnimNode_Clip : public AnimNode
{
public:
	AnimNode_Clip() {  }
	AnimNode_Clip(string name) : name(name) {}

public:
	virtual void Update(float delta, AnimParam params) override;
	virtual void Evaluate(AnimPose& outPose, Skeleton* skeleton, map<string, AnimationClip*> animClips, AnimParam params) override;
	virtual void GetCurTime(float& val) { val = time; }
	virtual void SetCurTime(float val) { time = val; }

	void SetLoop(bool val) { loop = val; }

protected:
	string name;
	float time = 0.0f;
	
	bool loop = true;

};

class AnimNode_Blend1D : public AnimNode
{
public:
	AnimNode_Blend1D() {}
	AnimNode_Blend1D(AnimNode* n1, AnimNode* n2) { nodeA = n1, nodeB = n2; }

public:
	virtual void Update(float delta, AnimParam params) override { nodeA->Update(delta, params); nodeB->Update(delta, params); }
	virtual void Evaluate(AnimPose& outPose, Skeleton* skeleton, map<string, AnimationClip*> animClips, AnimParam params) override;
	

protected:
	AnimNode* nodeA;
	AnimNode* nodeB;

	float alpha = 0.0f;

	AnimPose poseA;
	AnimPose poseB;

};

struct AnimState
{
	string name;

	AnimNode* node;

	vector<AnimTransition*> transitions;
};

struct BlendSample
{
	float x;
	float y;

	AnimNode_Clip* node;
};


class AnimNode_Blend2D : public AnimNode
{
public:
	virtual void Update(float delta, AnimParam params) override
	{
		for (auto& s : samples)
			s.node->Update(delta, params);
	}

	virtual void Evaluate(AnimPose& outPose, Skeleton* skeleton, map<string, AnimationClip*> animClips, AnimParam params) override;

protected:
	vector<BlendSample> samples;

	float paramX;
	float paramY;

};

class AnimNode_StateMachine : public AnimNode
{
public:
	virtual void Update(float delta, AnimParam params) override { states[currentState].node->Update(delta, params); }
	virtual void Evaluate(AnimPose& outPose, Skeleton* skeleton, map<string, AnimationClip*> animClips, AnimParam params) override;

	void SetState(string name) { currentState = name; }
	void AddState(string name, AnimState state) { states.insert(make_pair(name, state)); }

protected:
	map<string, AnimState> states;

	string currentState;

	float blendTime = 0.0f;
	float transitionTimer = 0.0f;

	AnimNode* prevNode = nullptr;

};

class AnimNode_LayerBlend : public AnimNode
{
public: 
	virtual void Evaluate(AnimPose& outPose, Skeleton* skeleton, map<string, AnimationClip*> animClips, AnimParam params) override;

private:
	bool IsChildOfMask(Skeleton* skeleton, int boneIndex);

private:
	AnimNode* baseNode = nullptr;
	AnimNode* layerNode = nullptr;

	int maskBoneIndex = -1;

};