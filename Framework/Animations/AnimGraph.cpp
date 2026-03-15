#include "AnimGraph.h"

bool AnimTransition::Checking(AnimParam & params, float curTime, float animDuration)
{
	switch (type)
	{
	case PARAM:
	{
		for (auto& c : conditions)
		{
			if (!c.second->Evaluate(params))
				return false;
		}
		return true;
	}
		break;
	case ANIM_END:
		return curTime >= animDuration;
		break;
	case TRIGGERTIME:
		return curTime >= triggerTime;
		break;
	default:
		break;
	}

	return false;
}


bool AnimCondition::Evaluate(AnimParam & params)
{
	switch (type)
	{
	case FLOAT_MORE:
		return *(params.floats.find(paramName)->second) > threshold;

	case FLOAT_LESS:
		return *(params.floats.find(paramName)->second) < threshold;

	case BOOL_TRUE:
		return *(params.bools.find(paramName)->second);

	case BOOL_FALSE:
		return !*(params.bools.find(paramName)->second);
	}

	return false;
}

AnimPose& AnimGraph::Update(Skeleton* skeleton, map<string, AnimationClip*>& animClips, AnimParam params, float delta)
{
	if (!rootNode)
		return finalPose;

	rootNode->Update(delta, params);
	rootNode->Evaluate(finalPose, skeleton, animClips, params);

	return finalPose;
}

AnimNode_Clip * AnimGraph::CreateClip(string name)
{
	auto clip = new AnimNode_Clip(name);
	return clip;
}

AnimNode_Blend1D * AnimGraph::CreateBlend1D(AnimNode * n1, AnimNode * n2)
{
	auto blend1D = new AnimNode_Blend1D(n1, n2);
	return blend1D;
}

AnimNode_StateMachine * AnimGraph::CreateStateMachine()
{
	auto stateMachine = new AnimNode_StateMachine();
	return stateMachine;
}

