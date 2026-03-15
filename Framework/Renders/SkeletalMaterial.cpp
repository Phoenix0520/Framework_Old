#include "SkeletalMaterial.h"

SkeletalMaterial::SkeletalMaterial(Texture * texture, Shader * shader)
	: albedoTex(texture), shader(shader)
{
	CreateTransformBuffer();
	CreateBoneBuffer();
	CreateLightingBuffer();
	CreateCameraBuffer();
	CreateShadowBuffer();
	CreateSampleState();
	CreateBlendState();
	CreateRasterState();
	CreateDepthState();

	//shadowMap = TEXTUREMANAGER->CreateShadowMap(1024, 1024);
}

SkeletalMaterial::SkeletalMaterial(Texture * albedo, Texture * normal, Texture * sub, Shader * shader)
	: albedoTex(albedo), normalTex(normal), subTex(sub), shader(shader)
{
	CreateTransformBuffer();
	CreateBoneBuffer();
	CreateLightingBuffer();
	CreateCameraBuffer();
	CreateShadowBuffer();
	CreateSampleState();
	CreateBlendState();
	CreateRasterState();
	CreateDepthState();

	//shadowMap = TEXTUREMANAGER->CreateShadowMap(1024, 1024);
}

SkeletalMaterial::~SkeletalMaterial()
{
	SafeRelease(transformBuffer);
	SafeRelease(boneBuffer);
	SafeRelease(samplerState);
	SafeRelease(blendState);
	SafeRelease(rasterState);
	SafeRelease(depthState);
}

void SkeletalMaterial::Bind()
{
	ID3D11DeviceContext* dc = D3D::GetDC();

	if (!shader)
		return;

	shader->Bind();

	dc->VSSetConstantBuffers(0, 1, &transformBuffer); // b0
	dc->VSSetConstantBuffers(1, 1, &boneBuffer);      // b1
	dc->VSSetConstantBuffers(4, 1, &shadowBuffer);

	dc->PSSetConstantBuffers(1, 1, &boneBuffer);      // b1
	dc->PSSetConstantBuffers(2, 1, &lightingBuffer);
	dc->PSSetConstantBuffers(3, 1, &cameraBuffer);

	dc->PSSetSamplers(0, 1, &samplerState);
	//dc->PSSetSamplers(1, 1, &shadowSampler);

	dc->OMSetBlendState(blendState, nullptr, 0xffffffff);

	if (wireFrame)
		dc->RSSetState(wireFrameRasterState);
	else
		dc->RSSetState(rasterState);

	dc->OMSetDepthStencilState(depthState, 0);

	vector<ID3D11ShaderResourceView*> srvs;

	if (albedoTex != nullptr)
		srvs.push_back(albedoTex->GetSRV());
	if (normalTex != nullptr)
		srvs.push_back(normalTex->GetSRV());
	if (subTex != nullptr)
		srvs.push_back(subTex->GetSRV());

	shader->SetSRV(srvs);
}

void SkeletalMaterial::UpdateTransformBuffer(Matrix V, Matrix P)
{
	TransformBuffer buffer = {};
	D3DXMatrixTranspose(&buffer.world, &transform->GetWorldMatrix());
	D3DXMatrixTranspose(&buffer.view, &V);
	D3DXMatrixTranspose(&buffer.proj, &P);

	UpdateTransformBuffer(buffer);
}

void SkeletalMaterial::UpdateTransformBuffer(TransformBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	D3D::GetDC()->Map(transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	memcpy_s(mapped_subresource.pData, sizeof(TransformBuffer), &buffer, sizeof(TransformBuffer));
	D3D::GetDC()->Unmap(transformBuffer, 0);
}

void SkeletalMaterial::UpdateBoneBuffer(const vector<Matrix>& boneMatrices)
{
	BoneBuffer buffer = {};

	for (size_t i = 0; i < boneMatrices.size(); i++)
	{
		D3DXMatrixTranspose(&buffer.boneMatrices[i], &boneMatrices[i]);
	}

	D3D11_MAPPED_SUBRESOURCE subRes;
	D3D::GetDC()->Map(boneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
	memcpy(subRes.pData, &buffer, sizeof(BoneBuffer));
	D3D::GetDC()->Unmap(boneBuffer, 0);
}

void SkeletalMaterial::UpdateShadowBuffer(Matrix lightViewProj)
{
	ShadowBuffer temp;
	temp.lightViewProj = lightViewProj;

	UpdateShadowBuffer(temp);
}

void SkeletalMaterial::UpdateShadowBuffer(ShadowBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE subRes;
	D3D::GetDC()->Map(shadowBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
	memcpy(subRes.pData, &buffer, sizeof(ShadowBuffer));
	D3D::GetDC()->Unmap(shadowBuffer, 0);
}

void SkeletalMaterial::UpdateLightingBuffer(UINT lightIndex, Vector3 lightPos, Vector3 lightDir, float lightRadius, float lightRange, float lightIntensity, Vector3 lightColor, int lightType, float innerCone, float outerCone)
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

void SkeletalMaterial::UpdateLightingBuffer(UINT index, LightDesc lightDesc)
{
	LightingBuffer buffer = lightBuf;
	buffer.lightDesc[index] = lightDesc;
	buffer.lightCount = index + 1;

	UpdateLightingBuffer(buffer);
}

void SkeletalMaterial::UpdateLightingBuffer(LightingBuffer buffer)
{
	lightBuf = buffer;
	
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	D3D::GetDC()->Map(lightingBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	memcpy_s(mapped_subresource.pData, sizeof(LightingBuffer), &lightBuf, sizeof(LightingBuffer));
	D3D::GetDC()->Unmap(lightingBuffer, 0);
}

void SkeletalMaterial::UpdateCameraBuffer(Matrix view, Matrix proj, Vector3 camPos)
{
	CameraBuffer temp;
	D3DXMatrixInverse(&temp.invView, nullptr, &view);
	D3DXMatrixInverse(&temp.invProj, nullptr, &proj);
	temp.cameraPos = camPos;
	temp.padding = 0.0f;

	UpdateCameraBuffer(temp);
}

void SkeletalMaterial::UpdateCameraBuffer(CameraBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	D3D::GetDC()->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	memcpy_s(mapped_subresource.pData, sizeof(CameraBuffer), &buffer, sizeof(CameraBuffer));
	D3D::GetDC()->Unmap(cameraBuffer, 0);
}

void SkeletalMaterial::SetDepthEnable()
{
	D3D::GetDC()->OMSetDepthStencilState(depthState, 0);
}

void SkeletalMaterial::SetDepthDisable()
{
	D3D::GetDC()->OMSetDepthStencilState(depthDisableState, 0);
}


void SkeletalMaterial::CreateTransformBuffer()
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

void SkeletalMaterial::CreateBoneBuffer()
{
	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;			// Data 의 용도 : CPU Write & GPU Read. GPU 에서 변경이 가능하다.
		desc.ByteWidth = sizeof(BoneBuffer);		// Data 갯수 입력
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Buffer 의 타입
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// Buffer 의 CPU 의 역할

		HRESULT hr = D3D::GetDevice()->CreateBuffer
		(
			&desc,
			nullptr,
			&boneBuffer
		);

		Check(hr);
	}
}

void SkeletalMaterial::CreateLightingBuffer()
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

void SkeletalMaterial::CreateCameraBuffer()
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

void SkeletalMaterial::CreateShadowBuffer()
{
	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;				// Data 의 용도 : CPU Write & GPU Read. GPU 에서 변경이 가능하다.
		desc.ByteWidth = sizeof(ShadowBuffer);		// Data 갯수 입력
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Buffer 의 타입
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// Buffer 의 CPU 의 역할

		HRESULT hr = D3D::GetDevice()->CreateBuffer
		(
			&desc,
			nullptr,
			&shadowBuffer
		);

		Check(hr);
	}
}

void SkeletalMaterial::CreateSampleState()
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

	ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));

	// desc 설정
	{
		// UV 좌표와 W 좌표
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		// 정규화된 좌표를 만들었을때 어떻게 작동할 것인지에 대한 플래그
		// WRAP = 원본픽셀로 나머지 부분을 채운다는 의미이다.
		// CLAMP = 마지막 픽셀로 채운다.
		// MIRROR = 인접한 픽셀을 반전시켜서 채운다.

		desc.BorderColor[0] = 1;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 1;
		// 외곽선을 나타낸다.

		desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		// 이전 현재 데이터를 비교하는 방법을 정하는 플래그
		// 기존 샘플과 4개의 값을 비교하여 통과시킬지 아닐지를 결정한다.

		desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT	;
		// Scaling 축소 및 확대 가 될때 평균값을 내서 미리 1/4 크기로 이미지를 구성한다.
		// MIN = 이미지 축소
		// MAG = 이미지 확대
		// POINT = 두개의 픽셀이 존재한다면 한픽셀은 폐기한다.
		// LINEAR = 두개의 픽셀의 평균값

		desc.MaxAnisotropy = 16;
		// 비등방성 필터링 (이방성 필터링)

		desc.MinLOD = FLT_MIN;
		desc.MaxLOD = FLT_MAX;
		// 거리에 따른 이미지 선명도

		desc.MipLODBias = 0.5f;
	}

	// Sampler State 생성
	{
		HRESULT hr = D3D::GetDevice()->CreateSamplerState
		(
			&desc,
			&shadowSampler
		);

		Check(hr);
	}
}

void SkeletalMaterial::CreateBlendState()
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

void SkeletalMaterial::CreateRasterState()
{
	// desc 생성
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	// desc 설정
	{
		desc.FillMode = D3D11_FILL_SOLID;
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
			&rasterState
		);

		Check(hr);
	}

	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	// desc 설정
	{
		desc.FillMode = D3D11_FILL_WIREFRAME;
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
			&wireFrameRasterState
		);

		Check(hr);
	}
}

void SkeletalMaterial::CreateDepthState()
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
