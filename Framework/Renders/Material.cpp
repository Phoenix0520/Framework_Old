#include "Material.h"

Material::Material(Texture * albedo, Shader * shader)
	: albedoTex(albedo), shader(shader)
{
	CreateTransformBuffer();
	CreateEffectBuffer();
	CreateLightingBuffer();
	CreateCameraBuffer();
	CreateSampleState();
	CreateBlendState();
	CreateRasterState();
	CreateDepthState();

	UpdateUVRect();
}

Material::Material(Texture * albedo, Texture* normal, Texture* orm, Shader * shader)
	: albedoTex(albedo), normalTex(normal), ormTex(orm), shader(shader)
{
	CreateTransformBuffer();
	CreateEffectBuffer();
	CreateLightingBuffer();
	CreateCameraBuffer();
	CreateSampleState();
	CreateBlendState();
	CreateRasterState();
	CreateDepthState();

	UpdateUVRect();
}

Material::~Material()
{
	SafeRelease(transformBuffer);
	SafeRelease(effectBuffer);
	SafeRelease(samplerState);
	SafeRelease(blendState);
	SafeRelease(rasterState);
	SafeRelease(depthState);
}

void Material::Bind()
{
	auto dc = D3D::GetDC();

	if (shader)
		shader->Bind();

	dc->VSSetConstantBuffers(0, 1, &transformBuffer);
	
	if (wireFrame)
		dc->RSSetState(wireFrameRasterState);
	else
		dc->RSSetState(rasterState);

	dc->PSSetSamplers(0, 1, &samplerState);
	dc->PSSetConstantBuffers(1, 1, &effectBuffer);
	dc->PSSetConstantBuffers(2, 1, &lightingBuffer);
	dc->PSSetConstantBuffers(3, 1, &cameraBuffer);

	dc->OMSetBlendState(blendState, nullptr, 0xffffffff);

	dc->OMSetDepthStencilState(depthState, 0);

	vector<ID3D11ShaderResourceView*> srvs;

	if (albedoTex != nullptr)
		srvs.push_back(albedoTex->GetSRV());
	if (normalTex != nullptr)
		srvs.push_back(normalTex->GetSRV());
	if (ormTex != nullptr)
		srvs.push_back(ormTex->GetSRV());
	
	shader->SetSRV(srvs);
}

void Material::BindGBuffer(GBuffer* gBuffer)
{
	auto dc = D3D::GetDC();

	dc->VSSetConstantBuffers(0, 1, &transformBuffer);

	// Raster / Depth
	dc->RSSetState(nullptr);
	dc->PSSetSamplers(0, 1, &samplerState);
	dc->OMSetBlendState(blendState, nullptr, 0xffffffff);
	dc->OMSetDepthStencilState(nullptr, 0);

	// Blend 끔

	// Texture 전달
	ID3D11ShaderResourceView* srvs[3] =
	{
		albedoTex ? albedoTex->GetSRV() : nullptr,
		normalTex ? normalTex->GetSRV() : nullptr,
		ormTex ? ormTex->GetSRV() : nullptr
	};

	assert(srvs[0] && "albedo SRV NULL");
	assert(srvs[1] && "normal SRV NULL");
	assert(srvs[2] && "orm SRV NULL");

	dc->PSSetShaderResources(0, 3, srvs);
}

void Material::UpdateTransformBuffer(Matrix view, Matrix proj)
{
	TransformBuffer buffer;

	D3DXMatrixTranspose(&buffer.world, &transform->GetWorldMatrix());
	D3DXMatrixTranspose(&buffer.view, &view);
	D3DXMatrixTranspose(&buffer.proj, &proj);
	
	auto dc = D3D::GetDC();

	D3D11_MAPPED_SUBRESOURCE mapped;
	dc->Map(transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &buffer, sizeof(buffer));
	dc->Unmap(transformBuffer, 0);

	dc->VSSetConstantBuffers(0, 1, &transformBuffer);
}

void Material::UpdateEffectBuffer(UINT path, Color color, float val1, float val2, float val3)
{
	EffectBuffer temp;
	
	temp.path = path;
	temp.val1 = val1;
	temp.val2 = val2;
	temp.val3 = val3;
	temp.color = color;

	UpdateEffectBuffer(temp);
}

void Material::UpdateEffectBuffer(EffectBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	D3D::GetDC()->Map(effectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	memcpy_s(mapped_subresource.pData, sizeof(EffectBuffer), &buffer, sizeof(EffectBuffer));
	D3D::GetDC()->Unmap(effectBuffer, 0);
}

void Material::UpdateLightingBuffer(UINT lightIndex, Vector3 lightPos, Vector3 lightDir, float lightRadius, float lightRange, float lightIntensity, Vector3 lightColor, int lightType, float innerCone, float outerCone)
{
	LightDesc temp;
	temp.lightPos = lightPos;
	D3DXVec3Normalize(&temp.lightDir, &lightDir);
	temp.lightIntensity = lightIntensity;
	temp.lightRadius = lightRadius;
	temp.lightColor = lightColor;
	temp.lightType = lightType;
	temp.lightRange = lightRange;
	temp.innerCone = cosf(Math::ToRadian(innerCone));
	temp.outerCone = cosf(Math::ToRadian(outerCone));

	UpdateLightingBuffer(lightIndex, temp);
}

void Material::UpdateLightingBuffer(UINT index, LightDesc lightDesc)
{
	LightingBuffer buffer;
	buffer.lightDesc[index] = lightDesc;

	UpdateLightingBuffer(buffer);
}

void Material::UpdateLightingBuffer(LightingBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	D3D::GetDC()->Map(lightingBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	memcpy_s(mapped_subresource.pData, sizeof(LightingBuffer), &buffer, sizeof(LightingBuffer));
	D3D::GetDC()->Unmap(lightingBuffer, 0);
}

void Material::UpdateCameraBuffer(Matrix view, Matrix proj, Vector3 camPos)
{
	CameraBuffer temp;
	D3DXMatrixInverse(&temp.invView, nullptr, &view);
	D3DXMatrixInverse(&temp.invProj, nullptr, &proj);
	temp.cameraPos = camPos;
	temp.padding = 0.0f;

	UpdateCameraBuffer(temp);
}

void Material::UpdateCameraBuffer(CameraBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	D3D::GetDC()->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	memcpy_s(mapped_subresource.pData, sizeof(CameraBuffer), &buffer, sizeof(CameraBuffer));
	D3D::GetDC()->Unmap(cameraBuffer, 0);
}

void Material::UpdateUVRect()
{
	uvRect.x = 0;
	uvRect.y = 0;
	uvRect.w = albedoTex->GetWidth();
	uvRect.h = albedoTex->GetHeight();

	float u0 = uvRect.x / albedoTex->GetWidth();
	float v0 = uvRect.y / albedoTex->GetHeight();
	float u1 = (uvRect.x + uvRect.w) / albedoTex->GetWidth();
	float v1 = (uvRect.y + uvRect.h) / albedoTex->GetHeight();

	uv[0] = Vector2(u0, v0);
	uv[1] = Vector2(u1, v0);
	uv[2] = Vector2(u1, v1);
	uv[3] = Vector2(u0, v1);
}

void Material::UpdateUVCircle(int radialSlice)
{
	if (radialSlice < 3) return; // 최소 삼각형

	float texW = albedoTex->GetWidth();
	float texH = albedoTex->GetHeight();

	float u0 = uvRect.x / texW;
	float v0 = uvRect.y / texH;
	float u1 = (uvRect.x + uvRect.w) / texW;
	float v1 = (uvRect.y + uvRect.h) / texH;

	circleUV.clear();
	circleUV.resize(radialSlice + 1); // 중심 포함

	// 중심 UV
	float centerU = (u0 + u1) * 0.5f;
	float centerV = (v0 + v1) * 0.5f;
	circleUV[0] = Vector2(centerU, centerV);

	// 둘레 UV
	for (int i = 0; i < radialSlice; i++)
	{
		float theta = Math::PI * 2.0f * i / radialSlice;

		float x = cosf(theta) * 0.5f + 0.5f; // 0~1
		float y = sinf(theta) * 0.5f + 0.5f;

		float u = u0 + x * (u1 - u0);
		float v = v0 + y * (v1 - v0);

		circleUV[i + 1] = Vector2(u, v);
	}
}

void Material::UpdateNormalMap()
{
}

void Material::UpdateOrm()
{
}

void Material::SetDepthEnable()
{
	D3D::GetDC()->OMSetDepthStencilState(depthState, 0);
}

void Material::SetDepthDisable()
{
	D3D::GetDC()->OMSetDepthStencilState(depthDisableState, 0);
}

void Material::CreateTransformBuffer()
{
	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;			// Data 의 용도 : CPU Write & GPU Read. GPU 에서 변경이 가능하다.
		desc.ByteWidth = sizeof(TransformBuffer);		// Data 갯수 입력
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Buffer 의 타입
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// Buffer 의 CPU 의 역할

		HRESULT hr = D3D::GetDevice()->CreateBuffer
		(
			&desc,
			nullptr,
			&transformBuffer
		);

		Check(hr);
	}
}

void Material::CreateEffectBuffer()
{
	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;				// Data 의 용도 : CPU Write & GPU Read. GPU 에서 변경이 가능하다.
		desc.ByteWidth = sizeof(EffectBuffer);			// Data 갯수 입력
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Buffer 의 타입
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// Buffer 의 CPU 의 역할

		HRESULT hr = D3D::GetDevice()->CreateBuffer
		(
			&desc,
			nullptr,
			&effectBuffer
		);

		Check(hr);
	}
}

void Material::CreateLightingBuffer()
{
	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;				// Data 의 용도 : CPU Write & GPU Read. GPU 에서 변경이 가능하다.
		desc.ByteWidth = sizeof(LightingBuffer);		// Data 갯수 입력
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Buffer 의 타입
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// Buffer 의 CPU 의 역할

		HRESULT hr = D3D::GetDevice()->CreateBuffer
		(
			&desc,
			nullptr,
			&lightingBuffer
		);

		Check(hr);
	}
}

void Material::CreateCameraBuffer()
{
	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;				// Data 의 용도 : CPU Write & GPU Read. GPU 에서 변경이 가능하다.
		desc.ByteWidth = sizeof(CameraBuffer);		// Data 갯수 입력
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Buffer 의 타입
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// Buffer 의 CPU 의 역할

		HRESULT hr = D3D::GetDevice()->CreateBuffer
		(
			&desc,
			nullptr,
			&cameraBuffer
		);

		Check(hr);
	}
}

void Material::CreateSampleState()
{
	// desc 생성
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));

	// desc 설정
	{
		// UV 좌표와 W 좌표
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		// 정규화된 좌표를 만들었을때 어떻게 작동할 것인지에 대한 플래그
		// WRAP = 원본픽셀로 나머지 부분을 채운다는 의미이다.
		// CLAMP = 마지막 픽셀로 채운다.
		// MIRROR = 인접한 픽셀을 반전시켜서 채운다.

		desc.BorderColor[0] = 1;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 1;
		// 외곽선을 나타낸다.

		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		// 이전 현재 데이터를 비교하는 방법을 정하는 플래그
		// 기존 샘플과 4개의 값을 비교하여 통과시킬지 아닐지를 결정한다.

		desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		// Scaling 축소 및 확대 가 될때 평균값을 내서 미리 1/4 크기로 이미지를 구성한다.
		// MIN = 이미지 축소
		// MAG = 이미지 확대
		// POINT = 두개의 픽셀이 존재한다면 한픽셀은 폐기한다.
		// LINEAR = 두개의 픽셀의 평균값

		desc.MaxAnisotropy = 16;
		// 비등방성 필터링 (이방성 필터링)

		desc.MaxLOD = FLT_MAX;
		desc.MinLOD = FLT_MIN;
		// 거리에 따른 이미지 선명도

		desc.MipLODBias = 0.5f;
	}

	// Sampler State 생성
	{
		HRESULT hr = D3D::GetDevice()->CreateSamplerState
		(
			&desc,
			&samplerState
		);

		Check(hr);
	}
}

void Material::CreateBlendState()
{
	// Alpha 값을 책정한다.

	// desc 생성
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	// desc 설정
	{
		// Blending 은 OM 단계에서 설정된다.
		desc.AlphaToCoverageEnable = false;					
		desc.IndependentBlendEnable = false;				

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;	
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		// Blending 의 표준 Flags

	}

	// BlendState 생성
	{
		HRESULT hr = D3D::GetDevice()->CreateBlendState
		(
			&desc,
			&blendState
		);

		Check(hr);
	}
}

void Material::CreateRasterState()
{
	// desc 생성
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	// desc 설정
	{
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.SlopeScaledDepthBias = 0.0f;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;

	}
	// Rasterizer State 생성
	{
		HRESULT hr = D3D::GetDevice()->CreateRasterizerState
		(
			&desc,
			&rasterState
		);

		Check(hr);
	}

	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	// desc 설정
	{
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.SlopeScaledDepthBias = 0.0f;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;

	}
	// Rasterizer State 생성
	{
		HRESULT hr = D3D::GetDevice()->CreateRasterizerState
		(
			&desc,
			&wireFrameRasterState
		);

		Check(hr);
	}
}

void Material::CreateDepthState()
{
	// 기본 Depth State (일반 3D용)
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = D3D::GetDevice()->CreateDepthStencilState(&desc, &depthState);
		Check(hr);
	}

	// Depth 끄는 State (겹침용)
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = FALSE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = D3D::GetDevice()->CreateDepthStencilState(&desc, &depthDisableState);
		Check(hr);
	}
}
