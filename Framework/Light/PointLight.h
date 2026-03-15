#pragma once

#include "Framework.h"
#include "LightBase.h"

class PointLight : public LightBase
{
public:
	PointLight(float radius);
	~PointLight();

public:
	virtual void Initialize();
	virtual void Destroy();

	virtual void Update(Matrix V, Matrix P);
	virtual void Render();

protected:

};