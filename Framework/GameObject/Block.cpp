#include "Block.h"

Block::Block()
{
	Texture* texture1 = TEXTUREMANAGER->LoadTexture(L"Glass_Albedo.png");
	Texture* texture2 = TEXTUREMANAGER->LoadTexture(L"Metal_Normal.png");
	Texture* texture3 = TEXTUREMANAGER->LoadTexture(L"Metal_Orm.png");
	Shader* shader = SHADERMANAGER->LoadShader(L"Final.hlsl");

	Material* material = new Material(texture1, texture2, texture3, shader);

	staticMesh = new StaticMeshCube();
	collider = new BoxCollider();

	staticMesh->SetTransform(transform);

	collider->SetTransform(transform);

	staticMesh->Initialize();
	staticMesh->SetMaterial(material);

	collider->Initialize();
}

Block::~Block()
{
	SAFE_DELETE(transform);
	SAFE_DELETE(staticMesh);
	SAFE_DELETE(collider);	
}

void Block::Update(Matrix V, Matrix P)
{
	//staticMesh->Update(V, P);
	//cylinder->Update(V, P);
	staticMesh->Update(V, P);

	collider->Update(V, P);
}

void Block::Render()
{
	//staticMesh->Render();
	//cylinder->Render();
	staticMesh->Render();

	//staticMesh->GetMaterial()->UpdateEffectBuffer(1, Color(0, 0, 0, 0.75f));

	collider->Render();
}

void Block::RenderGBuffer(GBuffer* gBuffer)
{
	staticMesh->RenderGBuffer(gBuffer);
}