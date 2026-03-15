#pragma once

#include "Framework.h"

#include "AnimationClip.h"
#include "AnimGraph.h"
#include "AnimDefs.h"

class SkeletalMesh;
class AnimationClip;
class AnimGraph;

class AnimationController
{
public:
	AnimationController() { animGraph = new AnimGraph(); }
	~AnimationController() { SAFE_DELETE(animGraph); }

public:
	FORCEINLINE virtual void Update(Skeleton* skeleton, AnimPose& outPose)		{ outPose = animGraph->Update(skeleton, animClips, animParam, DELTA); }
	FORCEINLINE virtual void Add(AnimationClip* animationClip, string name)		{ animClips.insert(make_pair(name, animationClip)); }

	FORCEINLINE void SetParam(string name, float* valArr)	{ animParam.floats.insert(make_pair(name, valArr)); }
	FORCEINLINE void SetParam(string name, bool* valArr)	{ animParam.bools.insert(make_pair(name, valArr)); }

	virtual void BuildGraph() = 0;

protected:
	map<string, AnimationClip*> animClips = map<string, AnimationClip*>();

	AnimGraph* animGraph;
	AnimParam animParam;

};