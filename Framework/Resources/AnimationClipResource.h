#pragma once

#include "Framework.h"

#include "Animations/AnimationClip.h"

class AnimationClip;

class AnimationClipResource
{
public:
	AnimationClipResource() { animClip = new AnimationClip(); }
	~AnimationClipResource() {};

public:
	bool LoadBinary(const string& path);

public:
	AnimationClip* GetAnimationClip() { return animClip; }

private:
	AnimationClip* animClip;

};