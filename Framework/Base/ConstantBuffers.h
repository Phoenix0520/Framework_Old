#pragma once

#include "Framework.h"

struct UVData
{
	Vector2 offset = Vector2(0.0f, 0.0f);
	Vector2 scale = Vector2(1.0f, 1.0f);
};

struct Rect
{
	float x;
	float y;
	float w;
	float h;
};

struct TransformBuffer
{
	Matrix world;
	Matrix view;
	Matrix proj;
};

struct EffectBuffer
{
	UINT path;
	float val1;
	float val2;
	float val3;
	Color color;
};

struct LightDesc
{
	Vector3 lightPos;
	float lightRadius;

	Vector3 lightColor;
	float lightIntensity;

	Vector3 lightDir;
	UINT lightType;

	float lightRange;
	float innerCone;
	float outerCone;
	float padding;
};

struct LightingBuffer
{
	UINT lightCount;
	Vector3 padding = Vector3(0,0,0);

	LightDesc lightDesc[8];
};

struct CameraBuffer
{
	Matrix invView;
	Matrix invProj;

	Vector3 cameraPos;
	float padding;
};

struct ShadowBuffer
{
	Matrix lightViewProj;
};

struct BoneBuffer
{
	BoneBuffer()
	{
		for (auto mat : boneMatrices)
			D3DXMatrixIdentity(&mat);
	}

	Matrix boneMatrices[1024];
};
