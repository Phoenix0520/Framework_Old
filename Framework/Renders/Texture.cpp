#include "Texture.h"

Texture::Texture(ID3D11ShaderResourceView* srv)
{
	this->srv = srv;
}

Texture::~Texture()
{
	
}

ShadowMap::ShadowMap(UINT width, UINT height)
{
	auto device = D3D::GetDevice();

	imageSize.x = (float)width;
	imageSize.y = (float)height;

	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = width;
	texDesc.Height = height;

	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;

	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;

	texDesc.SampleDesc.Count = 1;

	texDesc.BindFlags =
		D3D11_BIND_DEPTH_STENCIL |
		D3D11_BIND_SHADER_RESOURCE;

	device->CreateTexture2D(&texDesc, nullptr, &tex);

	//--------------------------------
	// DepthStencilView
	//--------------------------------

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;

	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	device->CreateDepthStencilView(
		tex,
		&dsvDesc,
		&dsv
	);

	//--------------------------------
	// ShaderResourceView
	//--------------------------------

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;

	srvDesc.ViewDimension =
		D3D11_SRV_DIMENSION_TEXTURE2D;

	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(
		tex,
		&srvDesc,
		&srv
	);
}

ShadowMap::~ShadowMap()
{
}
