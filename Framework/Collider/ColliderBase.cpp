#include "ColliderBase.h"

ColliderBase::ColliderBase()
{
	Texture* texture = TEXTUREMANAGER->LoadTexture(L"Collider_Red.png");
	Shader* shader = SHADERMANAGER->LoadShader(L"00_Custom.hlsl");
	material = new Material(texture, shader);
	material->SetWireFrame(true);
}

ColliderBase::~ColliderBase() {
	DeleteBuffers();
	SafeDelete(material);
}

void ColliderBase::CreateVertexBuffer() {
	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.ByteWidth = sizeof(VertexData) * vertexCount;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// data 설정
		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices.data();

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer);
		Check(hr);
	}
}

void ColliderBase::CreateIndexBuffer()
{
	//Create Index Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices.data();

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &subResource, &indexBuffer);
		Check(hr);
	}
}

void ColliderBase::SetMaterial(Material * material) {
	this->material = material;

	this->material->SetTransform(transform);

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	// Subresource 애 데이터를 넣을준비를한다.

	memcpy(subResource.pData, vertices.data(), sizeof(VertexData) * vertexCount);
	// Subresource 에 정점에 대한 데이터를 넣어놓는다.

	D3D::GetDC()->Unmap(vertexBuffer, 0);
}

void ColliderBase::DeleteBuffers() {
	indices.clear();
	vertices.clear();

	SafeRelease(indexBuffer);
	SafeRelease(vertexBuffer);
}

void ColliderBase::Update(Matrix V, Matrix P) {
	transform->UpdateWorldMatrix();
	if (!isVisible)
		return;
	material->UpdateTransformBuffer(V, P);
	
	if (!HOVERHIGHLIGHT)
		return;

	if (isHovered)
		material->UpdateEffectBuffer(SHADER_CLIPMASK, Color(0.0f, 1.0f, 0.0f, 1.0f), 0, 0, 0);
	else
		material->UpdateEffectBuffer(SHADER_NONE, Color(0, 0, 0, 1.0f), 0, 0, 0);
}

void ColliderBase::Render() {
	if (!isVisible)
		return;

	if (material != nullptr)
		material->Bind();

	UINT stride = sizeof(VertexData);
	UINT offset = 0;

	auto dc = D3D::GetDC();

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//material->SetDepthDisable();

	dc->DrawIndexed(indexCount, 0, 0);

	//material->SetDepthEnable();
}

void ColliderBase::Initialize()
{
	CreateVertexBuffer();
	CreateIndexBuffer();

	SetMaterial(material);
}

void ColliderBase::Destroy()
{
}

void ColliderBase::CreateMesh(vector<VertexData> vertices, vector<UINT> indices)
{
	this->vertices = vertices;
	this->indices = indices;

	vertexCount = vertices.size();
	indexCount = indices.size();
}


bool ColliderBase::Intersect(ColliderBase* other)
{

	return false;
}

bool ColliderBase::IntersectMouse(float& outDistance)
{
	Vector3 rayOrigin = Vector3();
	Vector3 rayDir = Vector3();
	MOUSE->GetMouseRay(rayOrigin, rayDir);

	Vector3 aabbMin, aabbMax;
	GetAABB(aabbMin, aabbMax);

	float tMin = 0.0f;
	float tMax = FLT_MAX;

	for (int i = 0; i < 3; ++i)
	{
		float origin = (&rayOrigin.x)[i];
		float dir = (&rayDir.x)[i];
		float minB = (&aabbMin.x)[i];
		float maxB = (&aabbMax.x)[i];

		if (fabs(dir) < 1e-6f)
		{
			if (origin < minB || origin > maxB)
				return false;
		}
		else
		{
			float invDir = 1.0f / dir;
			float t1 = (minB - origin) * invDir;
			float t2 = (maxB - origin) * invDir;

			if (t1 > t2) std::swap(t1, t2);

			tMin = max(tMin, t1);
			tMax = min(tMax, t2);

			if (tMin > tMax)
			{
				outDistance = FLT_MAX;
				return false;
			}
		}
	}

	outDistance = tMin;

	return true;
}