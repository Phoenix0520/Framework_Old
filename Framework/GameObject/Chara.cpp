#include "Chara.h"

#include "Software/FBXImporter.h"

Chara::Chara()
{
	SkeletalMeshResource* r1 = new SkeletalMeshResource();
	r1->LoadBinary("C:/Users/user/Desktop/wjdrndud/Framework/_Assets/_FBX/_Mesh/Ezreal_All.mesh");
	r1->SetResourceType(ResourceType::BLENDER);

	AnimationClipResource* anim1 = new AnimationClipResource();
	AnimationClipResource* anim2 = new AnimationClipResource();
	AnimationClipResource* anim3 = new AnimationClipResource();
	AnimationClipResource* anim4 = new AnimationClipResource();
	AnimationClipResource* anim5 = new AnimationClipResource();
	anim1->LoadBinary("C:/Users/user/Desktop/wjdrndud/Framework/_Assets/_FBX/_Anim/Ezreal_Idle.anim");
	anim2->LoadBinary("C:/Users/user/Desktop/wjdrndud/Framework/_Assets/_FBX/_Anim/Ezreal_Walk.anim");
	anim3->LoadBinary("C:/Users/user/Desktop/wjdrndud/Framework/_Assets/_FBX/_Anim/Ezreal_Attack.anim");
	anim4->LoadBinary("C:/Users/user/Desktop/wjdrndud/Framework/_Assets/_FBX/_Anim/Ezreal_Death.anim");
	anim5->LoadBinary("C:/Users/user/Desktop/wjdrndud/Framework/_Assets/_FBX/_Anim/Ezreal_Q.anim");

	model = new SkeletalMesh();
	model->SetResource(r1);
	model->AddAnimation(anim1, "IDLE");
	model->AddAnimation(anim2, "WALK");
	model->AddAnimation(anim3, "ATK");
	model->AddAnimation(anim4, "DEATH");
	model->AddAnimation(anim5, "SPELL_Q");

	Shader* shader = SHADERMANAGER->LoadShader(L"SkeletalMeshSpecular.hlsl", true);
	Texture* albedo = TEXTUREMANAGER->LoadTextureByAbsoultePath(L"C:/Users/user/Desktop/wjdrndud/Framework/_Assets/_FBX/Ezreal.png");
	Texture* normal = TEXTUREMANAGER->LoadTextureByAbsoultePath(L"C:/Users/user/Desktop/wjdrndud/Framework/_Assets/_FBX/Mesh.fbm/Sample_Normal.png");
	Texture* orm = TEXTUREMANAGER->LoadTextureByAbsoultePath(L"C:/Users/user/Desktop/wjdrndud/Framework/_Assets/_FBX/Mesh.fbm/Sample_ORM.png");

	SkeletalMaterial* material = new SkeletalMaterial(albedo, shader);
	model->SetTransform(transform);
	model->GetTransform()->SetScale(.1f, .1f, .1f);
	model->GetTransform()->SetPosition(0.f, 0.f, 0.f);
	model->GetTransform()->SetRotation(0.0f, 90.0f, 0.0f);

	model->SetMaterial(material);

	model->GetMaterial()->UpdateLightingBuffer(0, Vector3(0,0,0), Vector3(0, -0.5, -0.5), 0, 0, 5.0f, Vector3(1, 1, 1));

	for (UINT i = 0; i < 9; i++)
	{
		Ezreal_A* a = new Ezreal_A();
		aa.push_back(a);
	}

}

Chara::~Chara()
{
}

void Chara::Initialize()
{
	SetMoveState(WALK_IDLE);

	model->GetAnimController()->SetParam("MoveSpeed", &moveSpeed);
	model->GetAnimController()->SetParam("Moving", &moving);
	model->GetAnimController()->SetParam("Dead", &isDead);
	model->GetAnimController()->SetParam("Attacking", &attacking);
	model->GetAnimController()->SetParam("ATK_AnimSpeed", &attackSpeed);
	model->GetAnimController()->BuildGraph();
}

void Chara::Destroy()
{
	SAFE_DELETE(model);
}


void Chara::Update(Matrix V, Matrix P)
{

	Vector3 pos = transform->GetPosition();
	
	switch (moveState)
	{
	case WALK_IDLE:
		moving = false;
		break;
	case WALK_LEFT:
	case WALK_RIGHT:
	case WALK_FRONT:
	case WALK_BACK:
	case WALK_FRONTLEFT:
	case WALK_FRONTRIGHT:
	case WALK_BACKLEFT:
	case WALK_BACKRIGHT:
		transform->SetRotation(0.0f, (float)moveState, 0.0f);
		transform->Move(moveSpeed);
		moving = true;
		break;
	default:
		break;
	}

	static float aTime = 2.733f / attackSpeed;

	static float atkStopTime = aTime - 0.35f;

	if (attacking)
	{
		if (aTime <= DELTA)
		{
			aTime = 2.733f / attackSpeed;
			
			moveSpeed = 35.0f;
			attacking = false;
		}
		else if (aTime <= atkStopTime)
		{
			aTime -= DELTA;

			moveSpeed = 35.0f;
		}
		else
		{
			aTime -= DELTA;

			moveSpeed = 0.0f;
			moving = false;
		}
	}
	
	model->Update(V, P);
	Super::Update(V, P);
}

void Chara::Render()
{
	model->Render();

	Super::Render();
}

void Chara::Jump()
{
	
	
}
