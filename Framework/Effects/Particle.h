#pragma once

#include "Framework.h"


struct Particle
{
	Vector3 position;
	Vector3 velocity;

	float lifetime;
	float age;

	float size;
	Color color;

	bool alive;
};

class SpawnModule
{
public:
	static Vector3 GeneratePoint();
	static Vector3 GenerateSphere(float min, float max, float radius);
	static Vector3 GenerateCone(float coneAngle, float heightMin, float heightMax);
	static Vector3 GenerateBox(float min, float max);
	static Vector3 GenerateDirectional();

};

class ParticleEmitter
{
public:
	ParticleEmitter() {} 
	~ParticleEmitter() {}

public:
	void UpdateParticles(float delta);
	void SpawnParticle();

	void Spawn(float delta);
	void Update(float delta);

private:
	vector<Particle> particles;

	float spawnRate = 50.0f;
	float spawnAccumulator = 0.0f;

	float particleLifetime = 1.0f;
	float startSpeed = 3.0f;

	Vector3 position;	

};