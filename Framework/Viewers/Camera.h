#pragma once

#include "Framework.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

public:
	virtual void Update() = 0;

public:
	FORCEINLINE Vector3 GetPosition()		const { return position; }
	FORCEINLINE Vector3 GetRotation()		const { return rotation; }
	FORCEINLINE Vector3 GetRotationDegree() const { return rotation * 180.0f / (float)D3DX_PI; }

	FORCEINLINE Vector3 GetForwardVector()	const { return forward; }
	FORCEINLINE Vector3 GetUpVector()		const { return up; }
	FORCEINLINE Vector3 GetRightVector()	const { return right; }

	FORCEINLINE Matrix GetViewMatrix() const { return viewMatrix; }

	void SetPosition(Vector3 pos);
	void SetRotation(Vector3 rot);
	void SetRotationDegree(Vector3 rot);

	FORCEINLINE void SetPosition(float x, float y, float z)			{ SetPosition(Vector3(x, y, z)); }
	FORCEINLINE void SetRotation(float x, float y, float z)			{ SetRotation(Vector3(x, y, z)); }
	FORCEINLINE void SetRotationDegree(float x, float y, float z)	{ SetRotationDegree(Vector3(x, y, z)); }

protected:
	virtual void UpdateRotationMatrix();
	virtual void UpdateViewMatrix();

protected:
	Matrix viewMatrix;
	Matrix rotationMatrix;

	Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);
	Quaternion quaternion = Quaternion();

	Vector3 forward	= Vector3(0.0f, 0.0f, 1.0f);
	Vector3 up		= Vector3(0.0f, 1.0f, 0.0f);
	Vector3 right	= Vector3(1.0f, 0.0f, 0.0f);


};

class FixedCamera : public Camera	// 고정 카메라
{
public:
	FixedCamera() {}
	~FixedCamera() {}

public:
	virtual void Update();

private:

};

class FreeCamera : public Camera // 키보드, 마우스 조작으로 움직일수 있는 카메라
{
public:
	FreeCamera() {}
	~FreeCamera() {}

public:
	virtual void Update();

	FORCEINLINE void SetMoveSpeed(float speed)		{ moveSpeed = speed; }
	FORCEINLINE void SetRotationSpeed(float speed)	{ rotateSpeed = speed; }

private:
	float moveSpeed = 15.0f;
	float rotateSpeed = 60.0f;

	Vector3 curPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 oldPos = Vector3(0.0f, 0.0f, 0.0f);

	bool rotating = false;
};