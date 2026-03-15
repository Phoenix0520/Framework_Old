#include "StaticMeshCube.h"

StaticMeshCube::StaticMeshCube(float size)
{
	this->size = size;
}

StaticMeshCube::~StaticMeshCube()
{
	Destroy();
}

void StaticMeshCube::Initialize()
{
	MeshBuilder::AddCube(Vector3(0.0f, 0.0f, 0.0f), size);
	MeshBuilder::BuildStaticMesh(this);

	Super::Initialize();
}

void StaticMeshCube::Destroy()
{
	Super::Destroy();
}

void StaticMeshCube::CreateVertexBuffer()
{
	Super::CreateVertexBuffer();
}

void StaticMeshCube::CreateIndexBuffer()
{

	Super::CreateIndexBuffer();
}

void StaticMeshCube::Update(Matrix V, Matrix P)
{
	Super::Update(V, P);
}

void StaticMeshCube::Render()
{
	Super::Render();
}

void StaticMeshCube::RenderGBuffer(GBuffer * gBuffer)
{
	Super::RenderGBuffer(gBuffer);
}

void StaticMeshCube::SetMaterial(Material* material)
{

	Super::SetMaterial(material);
}

void StaticMeshCube::UpdateDetails(float size)
{
	Destroy();

	this->size = size;

	Initialize();
}
