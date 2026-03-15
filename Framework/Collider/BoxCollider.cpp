#include "BoxCollider.h"

BoxCollider::BoxCollider()
{
	
}

BoxCollider::~BoxCollider()
{
	Destroy();
}

void BoxCollider::Initialize()
{
	MeshBuilder::AddCube(Vector3(0, 0, 0), this->transform->GetScale());
	MeshBuilder::BuildStaticMesh(this);

	Super::Initialize();
}

void BoxCollider::Destroy()
{
	Super::Destroy();
}

void BoxCollider::CreateVertexBuffer()
{
	
	Super::CreateVertexBuffer();
}

void BoxCollider::CreateIndexBuffer()
{
	
	Super::CreateIndexBuffer();
}


void BoxCollider::Update(Matrix V, Matrix P)
{
	Super::Update(V, P);
}

void BoxCollider::Render()
{
	Super::Render();
}

void BoxCollider::GetAABB(Vector3 & outMin, Vector3 & outMax)
{
	Matrix world = transform->GetWorldMatrix();

    Vector3 localMin(-0.5f, -0.5f, -0.5f);
    Vector3 localMax( 0.5f,  0.5f,  0.5f);

    Vector3 corners[8] =
    {
        { localMin.x, localMin.y, localMin.z },
        { localMax.x, localMin.y, localMin.z },
        { localMin.x, localMax.y, localMin.z },
        { localMax.x, localMax.y, localMin.z },
        { localMin.x, localMin.y, localMax.z },
        { localMax.x, localMin.y, localMax.z },
        { localMin.x, localMax.y, localMax.z },
        { localMax.x, localMax.y, localMax.z },
    };

    outMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
    outMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < 8; i++)
    {
        Vector3 p;
        D3DXVec3TransformCoord(&p, &corners[i], &world);

        outMin.x = min(outMin.x, p.x);
        outMin.y = min(outMin.y, p.y);
        outMin.z = min(outMin.z, p.z);

        outMax.x = max(outMax.x, p.x);
        outMax.y = max(outMax.y, p.y);
        outMax.z = max(outMax.z, p.z);
    }
}

void BoxCollider::SetMaterial(Material * material)
{
	
	Super::SetMaterial(material);
}

