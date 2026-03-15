#include "LightBase.h"

LightBase::LightBase()
{
	transform = new Transform();

	lightDesc.lightPos = transform->GetPosition();
	lightDesc.lightDir = transform->GetForwardVector();
}

LightBase::~LightBase()
{
	SafeDelete(material);
}

void LightBase::CreateVertexBuffer()
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

		// data 설정
		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices.data();

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer);
		Check(hr);
	}
}

void LightBase::CreateIndexBuffer()
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

void LightBase::DeleteBuffer()
{
	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);

	vertices.clear();
	indices.clear();
}


void LightBase::Initialize()
{
	CreateVertexBuffer();
	CreateIndexBuffer();

	material->SetTransform(transform);

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	// Subresource 애 데이터를 넣을준비를한다.

	memcpy(subResource.pData, vertices.data(), sizeof(VertexData) * vertexCount);
	// Subresource 에 정점에 대한 데이터를 넣어놓는다.

	D3D::GetDC()->Unmap(vertexBuffer, 0);
}

void LightBase::Destroy()
{
	DeleteBuffer();
}

void LightBase::Update(Matrix V, Matrix P)
{
	if (!isActive)
		return;
	
	transform->UpdateWorldMatrix();

	lightDesc.lightPos = transform->GetPosition();
	lightDesc.lightDir = transform->GetForwardVector();

	if (!isVisible)
		return;

	material->UpdateTransformBuffer(V, P);
	material->UpdateCameraBuffer(V, P, CONTEXT->GetCamera()->GetPosition());
}

void LightBase::Render()
{
	if (!isActive)
		return;

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

	material->SetDepthDisable();

	dc->DrawIndexed(indexCount, 0, 0);

	material->SetDepthEnable();
}

void LightBase::UpdateDetails()
{
	Destroy();

	Initialize();

}

