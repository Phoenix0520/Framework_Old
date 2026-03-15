#pragma once

#include "Framework.h"

struct AnimPose
{
	Matrix& operator[] (int index) { return bones[index]; }

	vector<Matrix> bones;
};

enum AnimConditionType
{
	FLOAT_MORE,
	FLOAT_LESS,
	BOOL_TRUE,
	BOOL_FALSE
};

enum AnimPlayType
{
	PLAY_ONCE,
	PLAY_LOOP,
	PLAY_PINGPONG
};

struct AnimParam
{
	map<string, float*> floats;
	map<string, bool*> bools;
};