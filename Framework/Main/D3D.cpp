#include "D3D.h"

D3DDesc*				D3D::description	= new D3DDesc();
ID3D11Device*			D3D::device			= nullptr;
ID3D11DeviceContext*	D3D::deviceContext	= nullptr;
IDXGISwapChain*			D3D::swapChain		= nullptr;

ID3D11Texture2D*			D3D::dsvBackBuffer		= nullptr;
ID3D11RenderTargetView*		D3D::renderTargetView	= nullptr;
ID3D11DepthStencilView*		D3D::depthStencilView	= nullptr;

vector<D3DEnumAdapterInfo*> D3D::adapterInfoArr;
int D3D::selectedAdapterIndex = -1;

void D3D::Initialize()
{
	SetGPUInfo();	// 먼저 GPU 정보를 읽어내고

	CreateSwapChainAndDevice(); // COM Interface 구현
	CreateBackBuffer(description->screenWidth, description->screenHeight); // 그리고 나서 백 버퍼를 만들어준다
	init = true;
}

void D3D::Destroy()
{
	DeleteBackBuffer();	// 백버퍼를 지워준뒤

	if (swapChain != nullptr)
		swapChain->SetFullscreenState(false, nullptr); // 전체화면을 종료한다. : 이를 종료함으로서 리소스들이 해제된다

	SafeRelease(deviceContext);
	SafeRelease(device);
	SafeRelease(swapChain);
	// 그 후 COM Interface 들도 생성된 순서의 역순으로 해제를 해준다
}

void D3D::SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
	if (rtv == nullptr)
		rtv = renderTargetView;

	if (dsv == nullptr)
		dsv = depthStencilView;

	// rtv 와 dsv 를 새롭게 입력하지 않으면 기존 rtv 와 dsv 를 사용하겠다

	deviceContext->OMSetRenderTargets(1, &rtv, dsv); // Output merger 단계 : rtv 를 입력된 값으로 사용하고 dsv 도 입력된 값을 사용하겠다는것
}

void D3D::Clear(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
	if (rtv == nullptr)
		rtv = renderTargetView;

	if (dsv == nullptr)
		dsv = depthStencilView;

	deviceContext->ClearRenderTargetView(rtv, description->bgColor); // rtv 를 color 색상으로 지워내겠다
	deviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0); // dsv 또한 지워 내겠다 : dsv 를 지우는데 사용할 Flags 와 깊이를 지정해준다.
}

void D3D::Present()
{
	swapChain->Present(description->vsync == true ? 1 : 0, 0);
	// swapChain 의 백버퍼에 요소를 그려내겠다. 수직 동기화가 켜져있으면 수직 동기화 옵션을 넣어준다.
}

void D3D::ResizeScreen(float width, float height)
{
	if (!init)	// 초기화가 안되어있다면 무효화
		return;

	if (width < 1 || height < 1)	// 창크기가 0이하 일경우 무효화 한다
		return;

	description->screenWidth		= width;
	description->screenHeight	= height;
	// 현재 윈도우 설명에도 창 크기 변화를 적용시킨다

	DeleteBackBuffer();	// 백버퍼를 지워주고
	{
		HRESULT hr = swapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0); // ResizeBuffers 를 통해 버퍼 창 크기를 설정해준다
		assert(SUCCEEDED(hr));	// 오류 체크도 꼭 해준다.
	}
	CreateBackBuffer(width, height); // 다시 새로운 크기로 백버퍼를 만들어준다
}

void D3D::EnumerateAdatpters()
{
	IDXGIFactory1* factory;	// Factory 는 GPU 와 관련된 메서드들을 사용할 수 있는 클래스이다. 여기서 factory 뒤에 붙는 1은 인터페이스 식별자이다.

	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory);
	Check(hr);
	
	UINT index = 0; // 그래픽 어뎁터(드라이버) (GPU) 의 인덱스
	
	while (true) // 그래픽 어뎁터를 전부 찾을 떄까지 반복하는 반복문
	{
		IDXGIAdapter1* adapter;
		hr = factory->EnumAdapters1(index, &adapter);	// Factory로 부터 index번째 Adapter를 받아온다

		if (hr == DXGI_ERROR_NOT_FOUND) break; // 그래픽 카드를 못찾았을시 반복문 종료
		Check(hr); // 정상작동 되었는지 체크

		D3DEnumAdapterInfo* adapterInfo = new D3DEnumAdapterInfo();
		ZeroMemory(adapterInfo, sizeof(D3DEnumAdapterInfo));
		// 직접 만든 D3DEnumAdapterInfo 클래스 초기화
		
		adapter->GetDesc1(&adapterInfo->adapterDesc);
		adapterInfo->ordinal = index;
		adapterInfo->adapter = adapter;
		// 읽어온 어뎁터 정보 입력

		if (EnumerateAdatpterOutput(adapterInfo)) break; // 어뎁터 출력 정보 로드 : 실패시 종료

		adapterInfoArr.push_back(adapterInfo); // 어뎁터 정보 배열에 info 추가

		index++; // 다음 번호로 넘어간다
	}

	SafeRelease(factory); // 사용이 끝난 factory 해제
}

bool D3D::EnumerateAdatpterOutput(D3DEnumAdapterInfo* adapterInfo)
{
	IDXGIOutput* output = nullptr; // COM Interface 는 직접 초기화가 불가능 하므로 일단 nullptr 로 초기화한다

	HRESULT hr = adapterInfo->adapter->EnumOutputs(0, &output); // 그래픽 출력 정보 가져오기
	if (hr == DXGI_ERROR_NOT_FOUND) return false; // 찾기 실패시 실패 반환

	D3DEnumOutputInfo* outputInfo = new D3DEnumOutputInfo(); 
	ZeroMemory(outputInfo, sizeof(D3DEnumOutputInfo));
	// 직접 만든 Output 정보 클래스 초기화

	hr = output->GetDesc(&outputInfo->outputDesc); // 가져온 출력 정보의 Desc 를 만든 클래스에 할당
	Check(hr); // 오류 체크도 해준다

	outputInfo->output = output; // 만든 클래스에 출력 자체도 저장

	UINT modeNum = 0;	// 디스플레이 모드 개수
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM; // 디스플레이 포맷 : 8비트 RGBA(부호없는 정수) 를 사용하겠다는 뜻

	hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &modeNum, nullptr);	// 모드 개수 가져오기
	
	DXGI_MODE_DESC* displayModeArr = new DXGI_MODE_DESC[modeNum]; // DXGI_MODE_DESC 를 원소의 개수가 modeNum인 배열 형태로 동적 할당
	hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &modeNum, displayModeArr); // 모드 가져오기
	// 이런식으로 하는 이유는 : 이중 포인터 변환으로 가져오지 않고 객체 배열을 넣기 떄문에
	// 받아올 모드 수(GetDisplayModeList 가 반환하는 모드 수) 와 배열의 원소 수(우리가 만든 displayModeArr 의 배열 길이) 를 맞춰주어야 하기 때문이다

	for (UINT i = 0; i < modeNum; i++) // 모드의 수만큼 반복하는 반복문
	{
		bool isFit = true;
		isFit &= displayModeArr[i].Width	== description->screenWidth;
		isFit &= displayModeArr[i].Height	== description->screenHeight;
		// 어뎁터와 프로그램 창의 화면 크기가 맞는지를 체크한다

		// &= 연산자가 생소할 수 있다 :  코드를 좀 멋있게 쓰고 싶다면 위처럼 써보는것도 나쁘지않다
		
		/* [아래와 같은 내용이라고 보면 된다]
		
		bool isFit = false; 
		if (displayModeArr[i].Width == description->screenWidth && displayModeArr[i].Height == description->screenHeight)
			isFit = true;
		else
			isFit = false;

		or

		bool isFit = displayModeArr[i].Width == description->screenWidth && displayModeArr[i].Height == description->screenHeight ? true : false
		*/

		if (isFit)	// 어뎁터의 화면 크기와 창 정보의 화면 크기가 일치한다면
		{
			outputInfo->numerator	= displayModeArr[i].RefreshRate.Numerator;
			outputInfo->denominator = displayModeArr[i].RefreshRate.Denominator;
			// 해상도 주사율을 받아와 입력한다 : 하나의 변수로 사용하지 않고 둘로 나눈 이유는 디스플레이 표준이 그러하기 때문이다. 보통의 그래픽스 프로그래밍에선 분자와 분모로 나누어져 있다.
		}
	}

	adapterInfo->outputInfo = outputInfo; // 만든 출력 정보 클래스를 어뎁터 정보 클래스에 저장해 준다
	SafeDeleteArray(displayModeArr); // 사용이 끝난 출력 모드 배열을 소멸시킨다

	return true; // 정상적으로 종료됨을 알린다
}

void D3D::SetGPUInfo()
{
	EnumerateAdatpters(); // 어뎁터 정보 받아옴

	// 이후 GPU 정보에 대해 추가할 내용이 있다면 추가해준다.
}

void D3D::CreateSwapChainAndDevice()
{
	SafeRelease(device);
	SafeRelease(deviceContext);
	SafeRelease(swapChain);
	// Com Interface 해제 : 다시 만들 경우도 있기 때문이다

	DXGI_MODE_DESC modeDesc;
	{
		ZeroMemory(&modeDesc, sizeof(DXGI_MODE_DESC)); // 초기화

		modeDesc.Width				= 0; // 버퍼 창 너비 : 일단 0으로 초기화
		modeDesc.Height				= 0; // 버퍼 창 높이 : 일단 0으로 초기화
		modeDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;			// 색상 포맷 8비트 RGBA (부호없는 정수형)
		modeDesc.ScanlineOrdering	= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// 스캔라인 순서 : 설정 안함 (기본형으로 적용)
		modeDesc.Scaling			= DXGI_MODE_SCALING_UNSPECIFIED;		// 스케일링 모드 : 설정 안함 (기본형으로 적용)

		if (description->vsync) // 수직동기화가 켜져있다면
		{
			modeDesc.RefreshRate.Numerator		= adapterInfoArr[0]->outputInfo->numerator;
			modeDesc.RefreshRate.Denominator	= adapterInfoArr[0]->outputInfo->denominator;
			// 창 주사율을 어뎁터와 똑같이 맞춘다.
		}
		else
		{
			modeDesc.RefreshRate.Numerator		= 0;
			modeDesc.RefreshRate.Denominator	= 1;
			// 아닐경우 0과 1로 설정한다 : 기본 주사율을 사용하겠다는 의미
		}
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	{
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC)); // swapChain 정보 초기화

		swapChainDesc.BufferDesc			= modeDesc;
		swapChainDesc.BufferCount			= 1;								// 버퍼 수 : 더블 버퍼링일 때 1 그 이상일경우 추가하지만 보통 추가 하지않는다
		swapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 버퍼 사용처 설정 : 디스플레이를 출력받는 버퍼로 사용하겠다는것
		swapChainDesc.OutputWindow			= description->handle;				// 출력될 창의 핸들
		swapChainDesc.SampleDesc.Count		= 1;								// 샘플링 Desc 의 갯수 : 멀티 샘플링을 사용할 것인가 설정 안할시 1
		swapChainDesc.SampleDesc.Quality	= 0;								// 샘플링 안하므로 설정 안함
		swapChainDesc.Windowed				= !description->fullScreen;			// 창 모드인지 아닌지
		swapChainDesc.SwapEffect			= DXGI_SWAP_EFFECT_DISCARD;			// Swap 효과 : 스왑 버퍼링 이후 버퍼의 내용을 비워버리도록 설정
		swapChainDesc.Flags					= 0;								// 플래그 딱히 설정 안함
	}

	vector<D3D_FEATURE_LEVEL> featureLevelArr =	// 수준 레벨의 배열
	{
		D3D_FEATURE_LEVEL_11_1, // 여러 레벨이 있지만 우리는 11_1 레벨을 사용할 것이다
	};
	D3D_FEATURE_LEVEL featureLevel;	// 수준레벨을 반환받을 변수

	UINT creationFlags = D3D11_CREATE_DEVICE_DEBUG;

	UINT maxVideoMemory = 0;	// GPU 의 메모리 크기(VRAM) 변수 (단위 : 메가 바이트)
	for (UINT i = 0; i < adapterInfoArr.size(); ++i) // 어뎁터 수만큼 반복
	{
		if (adapterInfoArr[i]->adapterDesc.DedicatedVideoMemory > maxVideoMemory) // 가장 큰 VRAM 을 가진 GPU 찾기
		{
			selectedAdapterIndex = i; // 그 GPU의 index 를 선택된 GPU index에 저장
			maxVideoMemory = adapterInfoArr[i]->adapterDesc.DedicatedVideoMemory; // VRAM 크기에 어뎁터의 VRAM 크기 저장
		}
	}

	this->numerator		= swapChainDesc.BufferDesc.RefreshRate.Numerator;
	this->denominator	= swapChainDesc.BufferDesc.RefreshRate.Denominator;
	// 이 클래스에도 해상도 정보를 입력한다

	HRESULT hr = D3D11CreateDeviceAndSwapChain	// COM Interface 생성
	(
		NULL,								// 어뎁터 설정 : NULL 일시 기본 어뎁터를 사용하겠다는것 						
		D3D_DRIVER_TYPE_HARDWARE,			// 드라이버 타입 설정 : 현재 설정된 기본 GPU 를 사용하겠다는것 							
		NULL,								// 그래픽 소프트웨어 : 써드파티에서 제공되는 라이브러리 모듈의 핸들 안쓸거므로 NULL
		creationFlags,						// 2D+3D 혼재를 사용하겠다는 FLAG
		featureLevelArr.data(),				// 지원할 버전 데이터들
		featureLevelArr.size(),				// 위에 선언된 데이터의 크기
		D3D11_SDK_VERSION,					// 현재 SDK 버전
		&swapChainDesc,								// 스왑 체인의 정보
		&swapChain,							// 생성될 스왑체인 출력
		&device,							// 생성될 디바이스 출력
		&featureLevel,						// 생성될 버전 데이터 출력
		&deviceContext						// 생성될 디바이스 컨텍스트 출력
	);

	Check(hr); // 오류 체크

	// 직접 찾은 GPU를 사용할땐 아래와 같이 대체한다
	// 매개변수 1 : > adapterInfoArr[selectedAdapterIndex]->Adapter < 
	// 매개변수 2 : > D3D_DRIVER_TYPE_UNKNOWN <

	/* 직접찾은 GPU 를 사용하고 싶으면 아래를 사용한다
	this->numerator			= adapterInfoArr[selectedAdapterIndex]->outputInfo->numerator;	 // 해상도 분자
	this->denominator		= adapterInfoArr[selectedAdapterIndex]->outputInfo->denominator; // 해상도 분모
	this->gpuMemorySize		= apapterInfoArr[selectedAdapterIndex]->adapterDesc.DedicatedVideoMemory / 1024 / 1024; // 단위 : 바이트
	this->gpuDescription	= apapterInfoArr[selectedAdapterIndex]->adapterDesc.Description; // GPU 이름
	*/
}

void D3D::CreateBackBuffer(float width, float height)
{
	HRESULT hr;

	// RenderTargetView 생성
	{
		ID3D11Texture2D* rtvBackBuffer;
		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&rtvBackBuffer);
		Check(hr);
		// RTV 생성을 위한 버퍼 Texture2D 초기화 및 오류 체크

		hr = device->CreateRenderTargetView(rtvBackBuffer, NULL, &renderTargetView);
		Check(hr);
		// Device 에서 RTV 생성 및 오류 체크

		SafeRelease(rtvBackBuffer);
		// 사용이 끝난 backBuffer 해제
	}

	// DepthStencilView 생성
	{
		D3D11_TEXTURE2D_DESC textureDesc = { 0 };							// DSV 용 텍스처 버퍼는 기본 버퍼가 아니므로 직접 내용을 초기화한다
		textureDesc.Width				= (UINT)width;						// 창 너비
		textureDesc.Height				= (UINT)height;						// 창 높이
		textureDesc.MipLevels			= 1;								// Mip 레벨 : 사용하지 않으므로 1
		textureDesc.ArraySize			= 1;								// 텍스쳐 배열 크기 : 하나만 사용할 것 이므로 1 
		textureDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;	// 포맷 : 24비트 부호없는정수 Depth와 8비트 정수 Stencil 포맷을 사용한다
		textureDesc.Usage				= D3D11_USAGE_DEFAULT;				// 용도 : 일반적인 텍스처 버퍼로 사용하겠다.
		textureDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;			// 바인딩 플래그 : Depth Stencil에 바인딩하기 위한 텍스처이다
		textureDesc.CPUAccessFlags		= 0;								// CPU 플래그 : 동적 텍스처 (실시간 변화 텍스처)가 아니므로 0
		textureDesc.MiscFlags			= 0;								// 텍스처 기타 속성 : 사용안하므로 0
		textureDesc.SampleDesc.Count	= 1;								// 멀티 샘플링 사용안함
		textureDesc.SampleDesc.Quality = 0;								// 사용안하므로 0

		hr = device->CreateTexture2D(&textureDesc, NULL, &dsvBackBuffer);
		Check(hr);
		// DSV 생성을 위한 버퍼 Texture2D 생성 및 오류 체크
	
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		// DSV 의 정보 초기화

		dsvDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT; // 포맷 : 24비트 부호없는정수 Depth와 8비트 정수 Stencil 포맷을 사용한다
		dsvDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D; // 차원 : 텍스처 버퍼 타입 (2D or 1D)
		dsvDesc.Texture2D.MipSlice	= 0;							 // Mip 사용하지 않으므로 0

		hr = device->CreateDepthStencilView(dsvBackBuffer, &dsvDesc, &depthStencilView);
		Check(hr);
		// 생성한 버퍼와 Desc 를 바탕으로 DSV 생성

		SetRenderTarget(renderTargetView, depthStencilView);
		// 생성한 RTV 와 DSV 를 바탕으로 RenderTarget 설정
	}
}

void D3D::DeleteBackBuffer()
{
	SafeRelease(depthStencilView);
	SafeRelease(renderTargetView);
	SafeRelease(dsvBackBuffer);
	// 출력 버퍼들 해제
}
