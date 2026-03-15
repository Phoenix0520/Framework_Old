#pragma once

#include "Framework.h"

#include "Meshes/Skeletal/SkeletalMesh.h"

#include "Ezreal_A.h"

class SkeletalMesh;

enum MoveState
{
	WALK_IDLE = -1,
	WALK_LEFT = -90,
	WALK_RIGHT = 90,
	WALK_FRONT = 0,
	WALK_BACK = 180,
	WALK_FRONTLEFT = -45,
	WALK_FRONTRIGHT = 45,
	WALK_BACKLEFT = 225,
	WALK_BACKRIGHT = 135,

};

enum JumpState
{
	JUMP_IDLE = -1,
	JUMP_START,
	JUMP_UP,
	JUMP_FALL,
	JUMP_LANDING
};

class Chara : public GameObject
{
public:
	Chara();
	~Chara();

public:
	virtual void Initialize();
	virtual void Destroy();
	
	virtual void Update(Matrix V, Matrix P);
	virtual void Render();

public:
	void SetMoveState(MoveState val) { moveState = val; }

	SkeletalMaterial* GetMaterial() { return model->GetMaterial(); }

	void Jump();

	void Dead(bool val) { isDead = val; }
	void Atk() { attacking = true; }

private:
	SkeletalMesh* model;

	vector<Ezreal_A*> aa;

	MoveState moveState = WALK_IDLE;
	float moveSpeed = 35.0f;
	bool moving = false;

	bool isDead = false;

	bool attacking = false;
	float attackSpeed = 2.5f;


	JumpState jumpState = JUMP_IDLE;
	float jumpTime = 1.0f;
	float jumpPower = 5.0f;
	bool jumping = false;
};