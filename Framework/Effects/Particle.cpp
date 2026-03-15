#include "Particle.h"


void ParticleEmitter::Spawn(float delta)
{
	spawnAccumulator += spawnRate * delta;

	int count = (int)spawnAccumulator;
	spawnAccumulator -= count;

	for (int i = 0; i < count; i++)
		SpawnParticle();
}

void ParticleEmitter::UpdateParticles(float delta)
{
	for (auto& p : particles)
	{
		if (!p.alive) continue;

		p.age += delta;

		if (p.age > p.lifetime)
		{
			p.alive = false;
			continue;
		}

		p.position += p.velocity * delta;
	}
}

void ParticleEmitter::SpawnParticle()
{
	Particle p;

	p.position = position;

	p.velocity = Math::RandomDirection(-1.0f, 1.0f) * startSpeed;

	p.age = 0;
	p.lifetime = particleLifetime;
	p.size = 1.0f;

	p.alive = true;

	particles.push_back(p);
}


void ParticleEmitter::Update(float delta)
{
	Spawn(delta);
	UpdateParticles(delta);
}

Vector3 SpawnModule::GeneratePoint()
{
	return Vector3(0, 0, 0);
}

Vector3 SpawnModule::GenerateSphere(float min, float max, float radius)
{
	Vector3 dir = Math::RandomVec3(min, max);

	float mulRad = Math::Random(0.0f, radius);

	return dir * mulRad;
}

Vector3 SpawnModule::GenerateCone(float coneAngle, float heightMin, float heightMax)
{
	float theta = Math::Random(0.0f, Math::PI * 2.0f);
    float u = Math::Random(0.0f, 1.0f);

    float cosAngle = cosf(Math::ToRadian(coneAngle));
    float y = Math::Lerp(cosAngle, 1.0f, u);

    float sinPi = sqrtf(1.0f - y * y);

    float x = cosf(theta) * sinPi;
    float z = sinf(theta) * sinPi;

    Vector3 dir(x, y, z);

    float height = Math::Random(heightMin, heightMax);

    return dir * height;
}

Vector3 SpawnModule::GenerateBox(float min, float max)
{
	return Math::RandomVec3(min, max);
}

Vector3 SpawnModule::GenerateDirectional()
{
	return Vector3();
}
