#pragma once

#include "Framework.h"

#include "Base/Singleton.h"

class FreeCamera;
class Perspective;
class Viewport;

class Context : public Singleton<Context>
{
private:
	Context() {}
	Context(const Context& other) {}
	~Context()  {}

public:
	friend Singleton;

	virtual void Initialize() override;
	virtual void Destroy() override;

	virtual void Update() override;
	virtual void Render() override;

public:
	void ResizeScreen();

	FORCEINLINE FreeCamera*		GetCamera()		{ return camera; }
	FORCEINLINE Perspective*	GetProjection() { return projection; }
	FORCEINLINE Viewport*		GetViewport()	{ return viewport; }

	Matrix GetViewMatrix();
	Matrix GetProjectionMatrix();

	FORCEINLINE Color& GetAmbient()		{ return ambient; }
	FORCEINLINE Color& GetSpecular()	{ return specular; }
	
	FORCEINLINE Vector3& GetDirection() { return direction; }
	FORCEINLINE Vector3& GetPosition()	{ return position; }

private:
	FreeCamera*		camera		= nullptr;
	Perspective*	projection	= nullptr;
	Viewport*		viewport	= nullptr;

	Color ambient	= Color(0, 0, 0, 1);
	Color specular	= Color(1, 1, 1, 1);

	Vector3 direction	= Vector3(-1.0f, -1.0f, -1.0f);
	Vector3 position	= Vector3(0.0f, 0.0f, 0.0f);

};