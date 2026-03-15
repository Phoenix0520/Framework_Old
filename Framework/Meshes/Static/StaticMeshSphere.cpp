#include "StaticMeshSphere.h"

StaticMeshSphere::StaticMeshSphere(float radius, UINT radialStack, UINT radialSlice)
{
	this->radius = radius;
	this->radialStack = radialStack;
	this->radialSlice = radialSlice;
}

StaticMeshSphere::~StaticMeshSphere()
{
	Destroy();
}

void StaticMeshSphere::Initialize()
{
	MeshBuilder::AddSphere(Vector3(0, 0, 0), radius, radialStack, radialSlice);
	MeshBuilder::BuildStaticMesh(this);

	Super::Initialize();
}

void StaticMeshSphere::Destroy()
{
	Super::Destroy();
}

void StaticMeshSphere::Update(Matrix V, Matrix P)
{
	Super::Update(V, P);
}

void StaticMeshSphere::Render()
{
	Super::Render();
}

void StaticMeshSphere::SetMaterial(Material * material)
{
	Super::SetMaterial(material);
}


void StaticMeshSphere::CreateVertexBuffer()
{
	Super::CreateVertexBuffer();
}

void StaticMeshSphere::CreateIndexBuffer()
{
	Super::CreateIndexBuffer();
}

void StaticMeshSphere::UpdateDetails(float radius, UINT radialStack, UINT radialSlice)
{
	Destroy();

	this->radius = radius;
	this->radialStack = radialStack;
	this->radialSlice = radialSlice;

	Initialize();
}
