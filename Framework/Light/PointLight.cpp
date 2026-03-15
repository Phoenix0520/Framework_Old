#include "PointLight.h"

PointLight::PointLight(float radius)
{
	Texture* albedo = TEXTUREMANAGER->LoadTexture(L"PointLight_Red.png");

	Shader* shader = SHADERMANAGER->LoadShader(L"00_Custom.hlsl");
	material = new Material(albedo, shader);

	lightDesc.lightType = LIGHTTYPE_POINT;
	lightDesc.lightRadius = radius;
}

PointLight::~PointLight()
{
}

void PointLight::Initialize()
{
	MeshBuilder::AddSphere(Vector3(0, 0, 0), lightDesc.lightRadius, 124, 124);
	MeshBuilder::BuildStaticMesh(vertices, indices);

	vertexCount = vertices.size();
	indexCount = indices.size();

	Super::Initialize();
}

void PointLight::Destroy()
{
	Super::Destroy();
}

void PointLight::Update(Matrix V, Matrix P)
{
	Super::Update(V, P);

}

void PointLight::Render()
{
	Super::Render();
}
