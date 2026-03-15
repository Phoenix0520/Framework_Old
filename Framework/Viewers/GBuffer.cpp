#include "GBuffer.h"

GBuffer::GBuffer()
{
	
}

GBuffer::~GBuffer()
{
	Destroy();
}

void GBuffer::Initialize(UINT width, UINT height)
{
	auto device = D3D::GetDevice();

	DXGI_FORMAT formats[3] =
	{
		DXGI_FORMAT_R8G8B8A8_UNORM,     // Albedo
		DXGI_FORMAT_R16G16B16A16_FLOAT, // Normal
		DXGI_FORMAT_R8G8B8A8_UNORM      // ORM
	};

	for (int i = 0; i < 3; ++i)
	{	
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width	= width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = formats[i];
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags =
			D3D11_BIND_RENDER_TARGET |
			D3D11_BIND_SHADER_RESOURCE;

		device->CreateTexture2D(&desc, nullptr, &textures[i]);
		device->CreateRenderTargetView(textures[i], nullptr, &rtvs[i]);
		device->CreateShaderResourceView(textures[i], nullptr, &srvs[i]);
	}

	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.BindFlags =
		D3D11_BIND_DEPTH_STENCIL |
		D3D11_BIND_SHADER_RESOURCE;

	device->CreateTexture2D(&depthDesc, nullptr, &depthTex);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	device->CreateDepthStencilView(depthTex, &dsvDesc, &depthStencilView);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(depthTex, &srvDesc, &depthSrv);

	shader = SHADERMANAGER->LoadShader(L"GeometryBuffer.hlsl");
}

void GBuffer::Destroy()
{
	SafeRelease(depthSrv);
	SafeRelease(depthStencilView);
	SafeRelease(depthTex);

	for (int i = 0; i < 3; i++)
	{
		SafeRelease(srvs[i]);
		SafeRelease(rtvs[i]);
		SafeRelease(textures[i]);
	}
}

void GBuffer::SetRenderTargets()
{
	D3D::GetDC()->OMSetRenderTargets(3, rtvs, depthStencilView);
}

void GBuffer::Clear()
{
	auto dc = D3D::GetDC();

	Color clear = { 0,0,0,1 };

	float clearAlbedo[4] = { 0, 1, 0, 1 }; // ÃÊ·Ï
	float clearNormal[4] = { 1, 0, 0, 1 }; // »¡°­
	float clearPos[4] = { 0, 0, 1, 1 }; // ÆÄ¶û
	float clearOrm[4] = { 1, 1, 0, 1 }; // ³ë¶û

	dc->ClearRenderTargetView(rtvs[0], clear);
	dc->ClearRenderTargetView(rtvs[1], clear);
	dc->ClearRenderTargetView(rtvs[2], clear);

	dc->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);
}

void GBuffer::Bind()
{
	auto dc = D3D::GetDC();

	dc->OMSetRenderTargets(3, rtvs, depthStencilView);

	D3D11_VIEWPORT vp{};
	vp.Width = (FLOAT)D3D::GetWinWidth();
	vp.Height = (FLOAT)D3D::GetWinHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	dc->RSSetViewports(1, &vp);
}

void GBuffer::BindSRVS(UINT startSlot)
{
	auto dc = D3D::GetDC();

	dc->PSSetShaderResources(startSlot, 3, srvs);
	dc->PSSetShaderResources(startSlot + 3, 1, &depthSrv);
}

void GBuffer::UnbindSRVS()
{
	ID3D11ShaderResourceView* nulls[3]{};
	D3D::GetDC()->PSSetShaderResources(0, 3, nulls);
}

void GBuffer::BindShader()
{
	if (shader != nullptr)
	{
		shader->Bind();
	}
}

void GBuffer::ResizeScreen(UINT width, UINT height)
{
	Destroy();

	Initialize(width, height);
}
