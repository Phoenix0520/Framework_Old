#pragma once

#include "Framework.h"

class Transform
{
public:
	Transform() { position = Vector3(0.0f, 0.0f, 0.0f), rotation = Vector3(0.0f, 0.0f, 0.0f), scale = Vector3(1.0f, 1.0f, 1.0f); }
	Transform(Vector3 position, Vector3 rotation, Vector3 scale);
	~Transform() {}

public:
	void operator() (const Vector3 pos, const Vector3 rot, const Vector3 sc)
	{
		position = pos;
		rotation = rot;
		scale = sc;
	}

public:
	void	UpdateWorldMatrix();
	Matrix	GetWorldMatrix() const { return world; }

public:
	FORCEINLINE Vector3		GetPosition()		const { return position; }
	FORCEINLINE Vector3		GetRotation()		const { return rotation; }
	FORCEINLINE Quaternion	GetQuaternion()		const { return quaternion; }
	FORCEINLINE Vector3		GetScale()			const { return scale; }
	FORCEINLINE Vector3		GetForwardVector()	const { return forward; }
	FORCEINLINE Vector3		GetUpVector()		const { return up; }
	FORCEINLINE Vector3		GetRightVector()	const { return right; }

	FORCEINLINE void SetPosition(Vector3 val)							{ position = val; UpdateWorldMatrix(); }
	FORCEINLINE void SetPosition(float x, float y, float z)				{ position = Vector3(x, y, z); UpdateWorldMatrix(); }
	
	FORCEINLINE void SetQuaternion(Quaternion val)						{ quaternion = val; UpdateWorldMatrix(); }
	FORCEINLINE void SetQuaternion(Vector3 val, float w)				{ quaternion = Quaternion(val.x, val.y, val.z, w); UpdateWorldMatrix(); }
	FORCEINLINE void SetQuaternion(float x, float y, float z, float w)	{ quaternion = Quaternion(x, y, z, w); UpdateWorldMatrix(); }
	
	FORCEINLINE void SetScale(Vector3 val)								{ scale = val; UpdateWorldMatrix(); }
	FORCEINLINE void SetScale(float val)								{ scale = Vector3(val, val, val); UpdateWorldMatrix(); }
	FORCEINLINE void SetScale(float x, float y, float z)				{ scale = Vector3(x, y, z); UpdateWorldMatrix(); }


	void SetRotation(float pitch, float yaw, float roll);
	void SetRotation(Vector3 val);

	void Move(float speed);
	void Move(Vector3 dir, float speed);


private:
	Matrix		world;

	Vector3		position	= Vector3(0.0f, 0.0f, 0.0f);
	Vector3		rotation	= Vector3(0.0f, 0.0f, 0.0f);
	Quaternion	quaternion	= Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	Vector3		scale		= Vector3(1.0f, 1.0f, 1.0f);

	Vector3		forward		= Vector3(0.0f, 0.0f, 1.0f);
	Vector3		up			= Vector3(0.0f, 1.0f, 0.0f);
	Vector3		right		= Vector3(1.0f, 0.0f, 0.0f);

};