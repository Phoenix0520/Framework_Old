#include "Ezreal_A.h"

Ezreal_A::Ezreal_A()
{
	Texture* texture1 = TEXTUREMANAGER->LoadTexture(L"Metal_Albedo.png");
	Texture* texture2 = TEXTUREMANAGER->LoadTexture(L"Metal_Normal.png");
	Texture* texture3 = TEXTUREMANAGER->LoadTexture(L"Metal_Orm.png");
	Shader* shader = SHADERMANAGER->LoadShader(L"Final.hlsl");

	Material* material = new Material(texture1, texture2, texture3, shader);

	staticMesh = new StaticMeshSphere();
	collider = new BoxCollider();

	staticMesh->SetTransform(transform);

	collider->SetTransform(transform);

	staticMesh->Initialize();
	staticMesh->SetMaterial(material);

	collider->Initialize();

	staticMesh->GetMaterial()->UpdateLightingBuffer(0, Vector3(), Vector3(0, 1, 1), 0, 0, 10.0f);
}

Ezreal_A::~Ezreal_A()
{
	SAFE_DELETE(transform);
	SAFE_DELETE(staticMesh);
	SAFE_DELETE(collider);
}

void Ezreal_A::Update(Matrix V, Matrix P)
{
	transform->Move(200.0f);

	staticMesh->Update(V, P);

	collider->Update(V, P);
}

void Ezreal_A::Render()
{
	staticMesh->Render();

	collider->Render();
}

void Ezreal_A::RenderGBuffer(GBuffer * gBuffer)
{
}

void Ezreal_A::Shoot()
{
	shooting = true;
}
