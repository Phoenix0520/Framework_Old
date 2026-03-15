#include "Context.h"

void Context::Initialize()
{
	D3DDesc* desc = D3D::GetDesc();

	projection	= new Perspective(desc->screenWidth, desc->screenHeight);
	viewport	= new Viewport(desc->screenWidth, desc->screenHeight);
	camera		= new FreeCamera();
}

void Context::Destroy()
{
	SafeDelete(camera);
	SafeDelete(viewport);
	SafeDelete(projection);
}

void Context::Update()
{
	camera->Update();
}

void Context::Render()
{
	viewport->UpdateViewport();

	Vector3 rot = camera->GetRotationDegree();
	Vector3 mPos = MOUSE->GetPosition();
	Vector3 cPos = camera->GetPosition();
	Vector3 forward = camera->GetForwardVector();
	Vector3 up = camera->GetUpVector();
	Vector3 right = camera->GetRightVector();

	string str = String::Format("FPS : %.1f, DELTA : %.7f", TIME->GetFramePerSec(), DELTA);
	GUI->RenderText(5, 5, 1, 1, 1, str);

	str = String::Format("Mouse Position : %.1f, %.1f, %.1f", mPos.x, mPos.y, mPos.z);
	GUI->RenderText(5, 20, 1, 1, 1, str);

	str = String::Format("Cam Rotation : %.1f, %.1f, %.1f", rot.x, rot.y, rot.z);
	GUI->RenderText(5, 35, 1, 1, 1, str);

	str = String::Format("Cam Position : %.1f, %.1f, %.1f", cPos.x, cPos.y, cPos.z);
	GUI->RenderText(5, 50, 1, 1, 1, str);
}

void Context::ResizeScreen()
{
	projection->Set(D3D::GetWinWidth(), D3D::GetWinHeight());
	viewport->Set(D3D::GetWinWidth(), D3D::GetWinHeight());
}

Matrix Context::GetViewMatrix()
{
	return camera->GetViewMatrix();
}

Matrix Context::GetProjectionMatrix()
{
	return projection->GetProjectionMatrix();
}
