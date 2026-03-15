#pragma once

#include "Framework.h"

class Viewport
{
public:
	Viewport(float width, float height, float startX = 0.0f, float startY = 0.0f, float minDepth = 0.0f, float maxDepth = 1.0f);
	~Viewport();

public:
	void Set(float width, float height, float startX = 0.0f, float startY = 0.0f, float minDepth = 0.0f, float maxDepth = 1.0f);
	void UpdateViewport();

	FORCEINLINE float GetWidth()  const { return width; }
	FORCEINLINE float GetHeight() const { return height; }

	Vector3 WCToVC(Vector3 source, Matrix W, Matrix V, Matrix P);	// 월드 3D 좌표에서 화면 2D 좌표
	Vector3 VCToWC(Vector3 source, Matrix W, Matrix V, Matrix P);	// 화면 2D 좌표에서 월드 3D 좌표

private:
	float startX	= 0.0f;
	float startY	= 0.0f;
	float width		= 0.0f;
	float height	= 0.0f;
	float minDepth  = 0.0f;
	float maxDepth	= 0.0f;

	D3D11_VIEWPORT viewport;
};

