#include "StaticMeshCylinder.h"

StaticMeshCylinder::StaticMeshCylinder(float radius, float height, UINT radialSlice)
{
	this->radius = radius;
	this->height = height;
	this->radialSlice = radialSlice;
}

StaticMeshCylinder::~StaticMeshCylinder()
{
	Destroy();
}

void StaticMeshCylinder::Initialize()
{
	MeshBuilder::AddCylinder(Vector3(0, 0, 0), radius, height, radialSlice);
	MeshBuilder::BuildStaticMesh(this);

	Super::Initialize();
}

void StaticMeshCylinder::Destroy()
{
	Super::Destroy();
}

void StaticMeshCylinder::CreateVertexBuffer()
{
	
	Super::CreateVertexBuffer();
}

void StaticMeshCylinder::CreateIndexBuffer()
{
	
	Super::CreateIndexBuffer();
}

void StaticMeshCylinder::Update(Matrix V, Matrix P)
{

	Super::Update(V, P);
}

void StaticMeshCylinder::Render()
{
	Super::Render();
}

void StaticMeshCylinder::SetMaterial(Material * material)
{

	Super::SetMaterial(material);
}

void StaticMeshCylinder::UpdateDetails(float radius, float height, UINT radialSlice)
{
	Destroy();

	this->radius = radius;
	this->height = height;
	this->radialSlice = radialSlice;


	Initialize();
}

