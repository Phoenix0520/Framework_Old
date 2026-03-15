#pragma once

#include "Framework.h"
#include "LightBase.h"


class DirectionalLight : public LightBase
{
public:
	DirectionalLight();
	~DirectionalLight();

public:
	virtual void Initialize();
	virtual void Destroy();

	virtual void Update(Matrix V, Matrix P);
	virtual void Render();

protected:
	Matrix lightVP;

};