#pragma once

#include "Framework.h"
#include "LightBase.h"

class SpotLight : public LightBase
{
public:
	SpotLight(float radius, float lightRange, float innerCone, float outerCone);
	~SpotLight();

public:
	virtual void Initialize();
	virtual void Destroy();

	virtual void Update(Matrix V, Matrix P);
	virtual void Render();

protected:

};