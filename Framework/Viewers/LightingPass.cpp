#include "LightingPass.h"

LightingPass::LightingPass()
{
	
}

LightingPass::~LightingPass()
{
	Destroy();
}

void LightingPass::Initialize(UINT width, UINT height)
{
	this->width		= width;
	this->height	= height;

	CreateFullScreenQuad();
	CreateSamplerState();
	CreateDepthState();
	CreateRenderTarget();
	
	shader = SHADERMANAGER->LoadShader(L"DeferredLighting.hlsl");
}

void LightingPass::Destroy()
{
	SafeRelease(lightSRV);
	SafeRelease(lightRTV);
	SafeRelease(lightTex);
	
	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);
	SafeRelease(sampler);
	SafeRelease(depthDisableState);
}

void LightingPass::Render(GBuffer * gBuffer)
{
	auto dc = D3D::GetDC();

	ID3D11RenderTargetView* backRTV = D3D::GetRTV();
	dc->OMSetRenderTargets(1, &backRTV, nullptr);

	Color clearColor = Color(0,0,0,1);
	dc->ClearRenderTargetView(backRTV, clearColor);

	// Viewport ´Ù½Ã ¼³Á¤
	D3D11_VIEWPORT vp{};
	vp.Width = (float)D3D::GetWinWidth();
	vp.Height = (float)D3D::GetWinHeight();
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	dc->RSSetViewports(1, &vp);

	// Depth ²û
	dc->OMSetDepthStencilState(depthDisableState, 0);

	gBuffer->BindSRVS(0);	

	// Shader
	shader->Bind();

	// Sampler
	dc->PSSetSamplers(0, 1, &sampler);

	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dc->DrawIndexed(6, 0, 0);

	ID3D11ShaderResourceView* nulls[8] = {};
	dc->PSSetShaderResources(0, 8, nulls);

	gBuffer->UnbindSRVS();
}


void LightingPass::CreateFullScreenQuad()
{
	VertexTexture vertices[4] =
	{
		{ Vector3(-1, +1, 0), Vector2(0, 0) },
		{ Vector3(+1, +1, 0), Vector2(1, 0) },
		{ Vector3(+1, -1, 0), Vector2(1, 1) },
		{ Vector3(-1, -1, 0), Vector2(0, 1) },
	};

	UINT indices[6] = { 0, 1, 3, 1, 2, 3 };

	auto device = D3D::GetDevice();

	// VB
	{
		D3D11_BUFFER_DESC desc{};
		desc.ByteWidth = sizeof(vertices);
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data{};
		data.pSysMem = vertices;

		device->CreateBuffer(&desc, &data, &vertexBuffer);
	}

	// IB
	{
		D3D11_BUFFER_DESC desc{};
		desc.ByteWidth = sizeof(indices);
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data{};
		data.pSysMem = indices;

		device->CreateBuffer(&desc, &data, &indexBuffer);
	}
}

void LightingPass::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc{};
	desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	D3D::GetDevice()->CreateSamplerState(&desc, &sampler);
}

void LightingPass::CreateDepthState()
{
	D3D11_DEPTH_STENCIL_DESC desc = {};
	desc.DepthEnable = FALSE;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT hr = D3D::GetDevice()->CreateDepthStencilState(&desc, &depthDisableState);
	Check(hr);
}

void LightingPass::CreateRenderTarget()
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = this->width;
	desc.Height = this->height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags =
		D3D11_BIND_RENDER_TARGET |
		D3D11_BIND_SHADER_RESOURCE;

	auto device = D3D::GetDevice();

	device->CreateTexture2D(&desc, nullptr, &lightTex);
	device->CreateRenderTargetView(lightTex, nullptr, &lightRTV);
	device->CreateShaderResourceView(lightTex, nullptr, &lightSRV);
}


void LightingPass::ResizeScreen(UINT width, UINT height)
{
	Destroy();

	Initialize(width, height);
}
