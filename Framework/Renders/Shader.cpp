#include "Shader.h"

#include <filesystem>

Shader::~Shader()
{
	SafeRelease(vertexShader);
	SafeRelease(pixelShader);
	SafeRelease(inputLayout);
}

bool Shader::Initialize(const wstring & shaderFile, bool skeletal)
{
	wstring fullPath = L"../_Shaders/" + shaderFile;

	if (!Path::ExistFile(fullPath))
	{
		wstring str = L"Shader | File missing : " + fullPath;
		wstring errMsg = L"셰이더 오류 : 파일 ";
		errMsg.append(fullPath);
		errMsg.append(L"이 없습니다");
	
		MessageBoxW(NULL, errMsg.c_str(), L"Error", MB_OK);

		return false;
	}

	// 정점 셰이더 연결
	{
		ID3DBlob* blob = nullptr;

		HRESULT hr = D3DX11CompileFromFile
		(
			fullPath.c_str(),	// 파일명
			0,					// MACRO 사용안함
			0,					// Include 사용안함
			"VS",				// hlsl 파일에 정의된 함수명과 동일
			"vs_5_0",			// 사용된 셰이더 버전
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,					// flag1
			0,					// flag2
			0,					// thread pump
			&vsBlob,			// blob
			&blob,				// 에러메세지
			0			    	// 에러메세지 받는주소
		);

		Check(hr);

		hr = D3D::GetDevice()->CreateVertexShader
		(
			vsBlob->GetBufferPointer(),		// Blob 의 시작 주소
			vsBlob->GetBufferSize(),		// Blob 의 크기
			nullptr,						//
			&vertexShader					// VS 의 주소
		);

		Check(hr);
	}

	// 픽셀 셰이더 연결
	{
		HRESULT hr = D3DX11CompileFromFile
		(
			fullPath.c_str(),	// 파일명
			0,					// MACRO 사용안함
			0,					// Include 사용안함
			"PS",				// hlsl 파일에 정의된 함수명과 동일
			"ps_5_0",			// 사용된 셰이더 버전
			0,					// flag1
			0,					// flag2
			nullptr,			// thread pump
			&psBlob,			// blob
			nullptr,			// 에러메세지
			nullptr				// 에러메세지 받는주소
		);

		Check(hr);

		hr = D3D::GetDevice()->CreatePixelShader
		(
			psBlob->GetBufferPointer(),		// Blob 의 시작 주소
			psBlob->GetBufferSize(),		// Blob 의 크기
			nullptr,						//
			&pixelShader					// PS 의 주소
		);

		Check(hr);
	}

	if (skeletal)
		CreateInputLayoutForSkeletalMesh();
	else
		CreateInputLayout();

	SafeRelease(vsBlob);
	SafeRelease(psBlob);

	return true;
}

void Shader::Bind()
{
	auto dc = D3D::GetDC();
	
	dc->VSSetShader(vertexShader, nullptr, 0);
	dc->PSSetShader(pixelShader, nullptr, 0);
	
	dc->IASetInputLayout(inputLayout);
}

void Shader::SetSRV(vector<ID3D11ShaderResourceView*> srvs)
{
	while (srvs.size() < 3)
	{
		srvs.push_back(nullptr);
	}
	
	ID3D11ShaderResourceView** inputSRVS = srvs.data();
	
	D3D::GetDC()->PSSetShaderResources(0, srvs.size(), inputSRVS);
}

void Shader::CreateInputLayout()
{
	// 레이아웃 생성
	{
		D3D11_INPUT_ELEMENT_DESC layout[4];

		// Struct Vector3 (Vector4 in HLSL)
		{
			layout[0].SemanticName = "POSITION";					// 시멘틱스 네임
			layout[0].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;			// 포맷
			layout[0].InputSlot = 0;
			layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[0].AlignedByteOffset = 0;
			layout[0].InstanceDataStepRate = 0;
		}

		// Struct Vector3
		{
			layout[1].SemanticName = "NORMAL";					// 시멘틱스 네임
			layout[1].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;			// 포맷
			layout[1].InputSlot = 0;
			layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[1].AlignedByteOffset = 12;		// float2 * 3 ( x, y, x) + w (동차)
			layout[1].InstanceDataStepRate = 0;
		}

		// Struct Vector2
		{
			layout[2].SemanticName = "TEXCOORD";					// 시멘틱스 네임
			layout[2].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[2].Format = DXGI_FORMAT_R32G32_FLOAT;			// 포맷
			layout[2].InputSlot = 0;
			layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[2].AlignedByteOffset = 24;						
			layout[2].InstanceDataStepRate = 0;
		}

		// Struct Vector2
		{
			layout[3].SemanticName = "TANGENT";						// 시멘틱스 네임
			layout[3].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;			// 포맷
			layout[3].InputSlot = 0;
			layout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[3].AlignedByteOffset = 32;					
			layout[3].InstanceDataStepRate = 0;
		}

		// 정점이 어떻게 그려질지를 정한다.
		HRESULT hr = D3D::GetDevice()->CreateInputLayout
		(
			layout,						// desc name
			4,							// desc 갯수
			vsBlob->GetBufferPointer(), // 정점버퍼의 주소를 준다.
			vsBlob->GetBufferSize(),	// 버퍼의 크기
			&inputLayout
		);

		Check(hr);
	}
}

void Shader::CreateInputLayoutForSkeletalMesh()
{
	// 레이아웃 생성
	{
		D3D11_INPUT_ELEMENT_DESC layout[6];

		// Struct Vector3 (Vector4 in HLSL)
		{
			layout[0].SemanticName = "POSITION";					// 시멘틱스 네임
			layout[0].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;			// 포맷
			layout[0].InputSlot = 0;
			layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[0].AlignedByteOffset = 0;
			layout[0].InstanceDataStepRate = 0;
		}

		// Struct Vector3
		{
			layout[1].SemanticName = "NORMAL";					// 시멘틱스 네임
			layout[1].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;			// 포맷
			layout[1].InputSlot = 0;
			layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;		// float2 * 3 ( x, y, x) + w (동차)
			layout[1].InstanceDataStepRate = 0;
		}

		// Struct Vector2
		{
			layout[2].SemanticName = "TEXCOORD";					// 시멘틱스 네임
			layout[2].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[2].Format = DXGI_FORMAT_R32G32_FLOAT;			// 포맷
			layout[2].InputSlot = 0;
			layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			layout[2].InstanceDataStepRate = 0;
		}

		// Struct Vector2
		{
			layout[3].SemanticName = "TANGENT";						// 시멘틱스 네임
			layout[3].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;			// 포맷
			layout[3].InputSlot = 0;
			layout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			layout[3].InstanceDataStepRate = 0;
		}

		// Struct UINT4
		{
			layout[4].SemanticName = "BLENDINDICES";					// 시멘틱스 네임
			layout[4].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[4].Format = DXGI_FORMAT_R32G32B32A32_UINT;			// 포맷
			layout[4].InputSlot = 0;
			layout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			layout[4].InstanceDataStepRate = 0;
		}

		// Struct Float4
		{
			layout[5].SemanticName = "BLENDWEIGHT";					// 시멘틱스 네임
			layout[5].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[5].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;			// 포맷
			layout[5].InputSlot = 0;
			layout[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[5].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			layout[5].InstanceDataStepRate = 0;
		}


		// 정점이 어떻게 그려질지를 정한다.
		HRESULT hr = D3D::GetDevice()->CreateInputLayout
		(
			layout,						// desc name
			6,							// desc 갯수
			vsBlob->GetBufferPointer(), // 정점버퍼의 주소를 준다.
			vsBlob->GetBufferSize(),	// 버퍼의 크기
			&inputLayout
		);

		Check(hr);
	}
}
