#include "Camera.h"

Camera::Camera()
{
	D3DXMatrixIdentity(&rotationMatrix);
	D3DXMatrixIdentity(&viewMatrix);

	UpdateViewMatrix();
	UpdateRotationMatrix();
}

Camera::~Camera()
{
}

void Camera::SetPosition(Vector3 pos)
{
	position = pos;

	UpdateViewMatrix();
}

void Camera::SetRotation(Vector3 rot)
{
	rotation = rot;

	UpdateRotationMatrix();
	UpdateViewMatrix();
}

void Camera::SetRotationDegree(Vector3 rot)
{
	rotation = rot * Math::PI / 180.0f;

	UpdateRotationMatrix();
	UpdateViewMatrix();
}

void Camera::UpdateRotationMatrix()
{
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.y, rotation.x, rotation.z);


	Vector3 F = Vector3(0, 0, 1);
	Vector3 U = Vector3(0, 1, 0);
	Vector3 R = Vector3(1, 0, 0);

	D3DXVec3TransformNormal(&forward,	&F, &rotationMatrix);
	D3DXVec3TransformNormal(&up,		&U, &rotationMatrix);
	D3DXVec3TransformNormal(&right,		&R, &rotationMatrix);
}

void Camera::UpdateViewMatrix()
{
	Vector3 look = position + forward;

	D3DXMatrixLookAtLH(&viewMatrix, &position, &look, &up);
}

////////////////////////////////////////////////////////////////////////////

void FixedCamera::Update()
{
}

////////////////////////////////////////////////////////////////////////////

void FreeCamera::Update()
{
	if (KEYDOWN(VK_HOME))
	{
		SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		return;
	}

	if (!rotating && MOUSEDOWN(MBUTTON_R))
	{
		rotating = true;
		oldPos = MOUSE->GetPosition();
	}
	else if (MOUSEUP(MBUTTON_R))
	{
		rotating = false;
	}

	if (MOUSEPRESS(MBUTTON_R))
	{
		// 이동
		Vector3 pos = position;
		
		if (KEYPRESS('W'))
		{
			pos += GetForwardVector() * moveSpeed * DELTA;

			SetPosition(pos);
		}
		else if (KEYPRESS('S'))
		{
			pos -= GetForwardVector() * moveSpeed * DELTA;

			SetPosition(pos);
		}
		if (KEYPRESS('A'))
		{
			pos -= GetRightVector() * moveSpeed * DELTA;

			SetPosition(pos);
		}
		else if (KEYPRESS('D'))
		{
			pos += GetRightVector() * moveSpeed * DELTA;

			SetPosition(pos);
		}
		if (KEYPRESS('Q'))
		{
			pos -= GetUpVector() * moveSpeed * DELTA;

			SetPosition(pos);
		}
		else if (KEYPRESS('E'))
		{
			pos += GetUpVector() * moveSpeed * DELTA;

			SetPosition(pos);
		}

		// 회전

		Vector3 rot = rotation;

		if (rotating)
		{
			curPos = MOUSE->GetPosition();

			float gapY = curPos.x - oldPos.x;
			float gapX = curPos.y - oldPos.y;

			if (abs(gapX) > 1.5f)	// 극미량만 움직였을땐 적용하지 않기 위해서 0 대신 사용
			{
				rot.x += gapX / abs(gapX) * rotateSpeed * Math::PI / 180.0f * DELTA; 
			}
			if (abs(gapY) > 1.5f)
			{
				rot.y += gapY / abs(gapY) * rotateSpeed * Math::PI / 180.0f * DELTA;
			}
			// 이동이 발생한 방향으로 회전

			// 360도가 넘어가면 불필요한 값을 내림
			if (rot.x >= Math::ToRadian(360.0f))
			{
				rot.x -= Math::ToRadian(360.0f);
			}
			else if (rot.x <= Math::ToRadian(-360.0f))
			{
				rot.x += Math::ToRadian(360.0f);
			}

			if (rot.y >= Math::ToRadian(360.0f))
			{
				rot.y -= Math::ToRadian(360.0f);
			}
			else if (rot.y <= Math::ToRadian(-360.0f))
			{
				rot.y += Math::ToRadian(360.0f);
			}

			SetRotation(rot);

			oldPos = curPos;
		}
	}
}

