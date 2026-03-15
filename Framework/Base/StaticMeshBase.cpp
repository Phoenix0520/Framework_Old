#include "StaticMeshBase.h"

StaticMeshBase::StaticMeshBase()
{
	
}

StaticMeshBase::~StaticMeshBase()
{
	SafeDelete(material);
	DeleteBuffers();
}

void StaticMeshBase::CreateVertexBuffer()
{
	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.ByteWidth = sizeof(VertexData) * vertexCount;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, nullptr, &vertexBuffer);
		Check(hr);

		D3D11_MAPPED_SUBRESOURCE sub;
		D3D::GetDC()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		memcpy(sub.pData, vertices.data(), sizeof(VertexData) * vertexCount);
		D3D::GetDC()->Unmap(vertexBuffer, 0);
	}
}

void StaticMeshBase::CreateIndexBuffer()
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

void StaticMeshBase::UpdateVertexBuffer()
{
	D3D11_MAPPED_SUBRESOURCE sub;
	D3D::GetDC()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, vertices.data(), sizeof(VertexData) * vertexCount);
	D3D::GetDC()->Unmap(vertexBuffer, 0);
}

void StaticMeshBase::UpdateIndexBuffer()
{
	SafeRelease(indexBuffer);
	
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

void StaticMeshBase::DeleteBuffers()
{
	indices.clear();
	vertices.clear();

	SafeRelease(indexBuffer);
	SafeRelease(vertexBuffer); 
}

void StaticMeshBase::Create(vector<VertexData> vertices, vector<UINT> indices)
{
	this->vertices = vertices;
	this->indices = indices;

	vertexCount = vertices.size();
	indexCount = indices.size();
}

void StaticMeshBase::SetMaterial(Material * material)
{
	this->material = material;
	material->SetTransform(transform);

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	// Subresource 애 데이터를 넣을준비를한다.

	memcpy(subResource.pData, vertices.data(), sizeof(VertexData) * vertexCount);
	// Subresource 에 정점에 대한 데이터를 넣어놓는다.

	D3D::GetDC()->Unmap(vertexBuffer, 0);
}


void StaticMeshBase::Initialize()
{
	CreateVertexBuffer();
	CreateIndexBuffer();
}

void StaticMeshBase::Destroy()
{
}

void StaticMeshBase::Update(Matrix V, Matrix P)
{	
	transform->UpdateWorldMatrix();

	if (!isVisible)
		return;

	material->UpdateTransformBuffer(V, P);
	material->UpdateCameraBuffer(V, P, CONTEXT->GetCamera()->GetPosition());
}

void StaticMeshBase::Render()
{
	if (!isVisible)
		return;

	if (!material)
	{
		return;
	}

	material->Bind();

	UINT stride = sizeof(VertexData);
	UINT offset = 0;

	auto dc = D3D::GetDC();

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dc->DrawIndexed(indexCount, 0, 0);
}


void StaticMeshBase::RenderGBuffer(GBuffer* gBuffer)
{
	if (!gBuffer || !material)
	{
		assert(1);
		return;
	}
	
	//gBuffer->Bind();
	//gBuffer->BindShader();
	material->BindGBuffer(gBuffer);

	UINT stride = sizeof(VertexData);
	UINT offset = 0;

	auto dc = D3D::GetDC();

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	dc->DrawIndexed(indexCount, 0, 0);
}


