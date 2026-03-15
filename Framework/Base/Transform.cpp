#include "Transform.h"

Transform::Transform(Vector3 position, Vector3 rotation, Vector3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	D3DXQuaternionIdentity(&quaternion);
	D3DXMatrixIdentity(&world);
}

void Transform::UpdateWorldMatrix()
{
	Matrix	S, R, T;

	Vector3 scale	 = GetScale();
	Vector3 rotation = GetRotation();
	Vector3 position = GetPosition();

	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);

	forward = Math::RotateVector(quaternion, Vector3(0, 0, 1));
	right = Math::RotateVector(quaternion, Vector3(1, 0, 0));
	up = Math::RotateVector(quaternion, Vector3(0, 1, 0));
	
	D3DXVec3Normalize(&forward, &forward);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Normalize(&up, &up);

	D3DXMatrixRotationQuaternion(&R, &quaternion);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	world = S * R * T;
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
	rotation = Vector3(pitch, yaw, roll);
	D3DXQuaternionRotationYawPitchRoll(&quaternion, Math::ToRadian(yaw), Math::ToRadian(pitch), Math::ToRadian(roll));
	D3DXQuaternionNormalize(&quaternion, &quaternion);
}

void Transform::SetRotation(Vector3 val)
{
	rotation = val;
	SetRotation(val.y, val.x, val.z);
}

void Transform::Move(float speed)
{
	if (speed <= 0.0001f)
		return;

	position += forward * speed * DELTA;
}

void Transform::Move(Vector3 dir, float speed)
{
	if (speed <= 0.0001f)
		return;

	Vector3 worldDir = Math::RotateVector(quaternion, dir);

	position += worldDir * speed * DELTA;
}