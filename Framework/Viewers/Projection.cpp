#include "Projection.h"

Projection::Projection(float width, float height, float zn, float zf, float fov)
{
	Set(width, height, zn, zf, fov);
	D3DXMatrixIdentity(&projMatrix);
}

Projection::~Projection()
{
}

void Projection::Set(float width, float height, float znear, float zfar, float fov)
{
	this->width		= width;
	this->height	= height;
	this->znear		= znear;
	this->zfar		= zfar;
	this->fov		= fov;
}

Orthographic::Orthographic(float width, float height, float znear, float zfar)
	: Projection(width, height, znear, zfar, 0.0f)
{
	Set(width, height, znear, zfar, 0.0f);
}

Orthographic::~Orthographic()
{
}

void Orthographic::Set(float width, float height, float znear, float zfar, float fov)
{
	Super::Set(width, height, znear, zfar, fov);

	D3DXMatrixOrthoLH(&projMatrix, width, height, znear, zfar);
	// 직교사영
}

Perspective::Perspective(float width, float height, float znear, float zfar, float fov)
	: Projection(width, height, znear, zfar, fov)
{
	Set(width, height, znear, zfar, fov);
}

Perspective::~Perspective()
{
}

void Perspective::Set(float width, float height, float znear, float zfar, float fov)
{
	Super::Set(width, height, znear, zfar, fov);

	aspect = width / height;
	D3DXMatrixPerspectiveFovLH(&projMatrix, fov, aspect, znear, zfar);
	// 투영사영
}
