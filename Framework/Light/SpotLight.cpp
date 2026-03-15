#include "SpotLight.h"

SpotLight::SpotLight(float radius, float range, float innerCone, float outerCone)
{
	Texture* albedo = TEXTUREMANAGER->LoadTexture(L"SpotLight_Red.png");

	Shader* shader = SHADERMANAGER->LoadShader(L"00_Custom.hlsl");
	material = new Material(albedo, shader);

	lightDesc.lightRadius = radius;
	lightDesc.lightRange = range;
	lightDesc.innerCone = innerCone;
	lightDesc.outerCone = outerCone;

	lightDesc.lightType = LIGHTTYPE_SPOT;
}

SpotLight::~SpotLight()
{
}

void SpotLight::Initialize()
{
	Vector3 scale = transform->GetScale();

	MeshBuilder::AddConeByCosAngle(Vector3(0, 0, 0), transform->GetForwardVector(), lightDesc.lightRange, lightDesc.innerCone, 16);
	MeshBuilder::AddConeByCosAngle(Vector3(0, 0, 0), transform->GetForwardVector(), lightDesc.lightRange, lightDesc.outerCone, 16);

	MeshBuilder::BuildStaticMesh(vertices, indices);

	vertexCount = vertices.size();
	indexCount = indices.size();

	//material->SetWireFrame(true);

	Super::Initialize();
}

void SpotLight::Destroy()
{
	Super::Destroy();
}

void SpotLight::Update(Matrix V, Matrix P)
{

	Super::Update(V, P);
}

void SpotLight::Render()
{
	Super::Render();
}
