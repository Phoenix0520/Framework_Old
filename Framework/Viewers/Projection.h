#pragma once

#include "Framework.h"

class Projection
{
public:
	Projection(float width, float height, float znear, float zfar, float fov);
	virtual ~Projection();

public:
	FORCEINLINE Matrix GetProjectionMatrix() const { return projMatrix; }

protected:
	virtual void Set(float width, float height, float znear, float zfar, float fov);

protected:
	float width		= 0.0f;
	float height	= 0.0f;
	float znear		= 0.0f;
	float zfar		= 0.0f;
	float fov		= 0.0f;

	Matrix projMatrix;
};

class Orthographic : public Projection
{
public:
	Orthographic(float width, float height, float znear = 0.1f, float zfar = 1000.0f);
	~Orthographic();

public:
	virtual void Set(float width, float height, float zn = 0.1f, float zf = 1000.0f, float fov = 0.0f) override;

private:


};

class Perspective : public Projection
{
public:
	Perspective(float width, float height, float znear = 0.1f, float zfar = 1000.0f, float fov = (float)D3DX_PI * 0.25f);
	~Perspective();

public:
	virtual void Set(float width, float height, float znear = 0.1f, float zfar = 1000.0f, float fov = (float)D3DX_PI * 0.25f) override;

private:
	float aspect = 0.0f;	// È­¸éºñ
};