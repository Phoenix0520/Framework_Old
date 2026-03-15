#pragma once

#include "Framework.h"
#include "Particle.h"

class Effect
{
public:
	void Update(float delta)
	{
		for (auto e : emitters)
			e->Update(delta);
	}


private:
	vector<ParticleEmitter*> emitters;

};