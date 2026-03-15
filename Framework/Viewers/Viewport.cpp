#include "Viewport.h"

Viewport::Viewport(float width, float height, float startX, float startY, float minDepth, float maxDepth)
{
	Set(width, height, startX, startY, minDepth, maxDepth);
}

Viewport::~Viewport()
{
}

void Viewport::Set(float width, float height, float startX, float startY, float minDepth, float maxDepth)
{
	viewport.TopLeftX	= this->startY = startY;
	viewport.TopLeftY	= this->startY = startY;
	viewport.Width		= this->width = width;
	viewport.Height		= this->height = height;
	viewport.MinDepth	= this->minDepth = minDepth;
	viewport.MaxDepth	= this->maxDepth = maxDepth;

	UpdateViewport();
}

void Viewport::UpdateViewport()
{
	D3D::GetDC()->RSSetViewports(1, &viewport);
}

Vector3 Viewport::WCToVC(Vector3 source, Matrix W, Matrix V, Matrix P)
{
	Vector3 position = source;
	Vector3 retval = Vector3(0.0f, 0.0f, 0.0f);

	Matrix WVP = W * V * P;	
	D3DXVec3TransformCoord(&retval, &position, &WVP);
	// World, View, Projection 매트릭스를 합친 전역 world 공간으로 변환

	retval.x = ((retval.x + 1.0f) * 0.5f) * width + startX;
	retval.y = ((-retval.y + 1.0f) * 0.5f) * height + startY;
	retval.z = (retval.z * (maxDepth - minDepth)) + minDepth;
	// 현재 뷰포트의 크기를 토대로 좌표의 위치를 정한다

	return retval;
}

Vector3 Viewport::VCToWC(Vector3 source, Matrix W, Matrix V, Matrix P)
{
	Vector3 position = source;
	Vector3 retval = Vector3(0.0f, 0.0f, 0.0f);

	retval.x = ((position.x - startX) / width) * 2.0f - 1.0f;
	retval.y = (((position.y - startY) / height) * 2.0f - 1.0f) * -1.0f;
	retval.z = ((position.z - minDepth) / (maxDepth - minDepth));
	// 다시 현재 뷰포트의 크기를 빼서 좌표를 구한다

	Matrix WVP = W * V * P;
	D3DXMatrixInverse(&WVP, NULL, &WVP);
	// World 공간 매트릭스의 역치를 구하고

	D3DXVec3TransformCoord(&retval, &retval, &WVP);
	// 이를 곱해 다시 그 좌표계로 변환한다

	return retval;
}