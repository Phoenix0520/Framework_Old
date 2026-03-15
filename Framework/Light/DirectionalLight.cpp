#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	Texture* albedo = TEXTUREMANAGER->LoadTexture(L"Red.png");

	Shader* shader = SHADERMANAGER->LoadShader(L"00_Custom.hlsl");
	material = new Material(albedo, shader);

	lightDesc.lightType = LIGHTTYPE_POINT;
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Initialize()
{
	Vector3 scale = transform->GetScale();

	Quaternion q;
	
	MeshBuilder::AddCylinder(Vector3(0, -scale.y * 0.25f, 0), scale.x * 0.05f, scale.y * 0.5f, 16);
	MeshBuilder::AddCone(Vector3(0, +scale.y * 0.25f, 0), scale.x * 0.1f, scale.y * 0.5f, 16);

	MeshBuilder::BuildStaticMesh(vertices, indices);

	vertexCount = vertices.size();
	indexCount = indices.size();

	Super::Initialize();
}

void DirectionalLight::Destroy()
{
	Super::Destroy();
}

void DirectionalLight::Update(Matrix V, Matrix P)
{
	Super::Update(V, P);
}

void DirectionalLight::Render()
{
	Super::Render();
}
