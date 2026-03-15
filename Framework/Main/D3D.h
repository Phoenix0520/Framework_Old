#pragma once

#include "Framework.h"
#include "Base/Singleton.h"

struct D3DDesc // 프로그램 창의 정보 구조체
{
	wstring		appName;		// 프로그램 이름
	HINSTANCE	instance;		// 윈도우 인스턴스
	HWND		handle;			// 윈도우 핸들
	float		screenWidth;	// 창 너비
	float		screenHeight;	// 창 높이
	bool		vsync;			// VSync(수직 동기화) 모드 인지
	bool		fullScreen;		// 전체화면모드 인지
	Color		bgColor;		// 배경 색
};

class D3DEnumOutputInfo
{
public:
	D3DEnumOutputInfo() {}
	~D3DEnumOutputInfo() { SafeRelease(output); }

public:
	IDXGIOutput* output = nullptr;	// 어뎁터 출력
	DXGI_OUTPUT_DESC	outputDesc;			// 출력 정보

	UINT				numerator = 0;		// 어뎁터 주사율 분자
	UINT				denominator = 1;	// 어뎁터 주사율 분모

private:
	const D3DEnumOutputInfo& operator=(const D3DEnumOutputInfo& rhs)
	{
		
	}
};

class D3DEnumAdapterInfo	// 그래픽 카드 정보
{
public:
	D3DEnumAdapterInfo() {}
	~D3DEnumAdapterInfo() { SafeRelease(adapter); SafeDelete(outputInfo); }

public:
	UINT				ordinal;				// 어뎁터 번호
	IDXGIAdapter1* adapter = nullptr;	// 어뎁터 객체
	DXGI_ADAPTER_DESC1  adapterDesc;			// 어뎁터 정보

	D3DEnumOutputInfo* outputInfo;				// 출력 정보

private:
	const D3DEnumAdapterInfo& operator=(const D3DEnumAdapterInfo& other); // 깊은복사용
};


class D3D : public Singleton<D3D>
{
private: 
	D3D() {}
	D3D(const D3D& other) {}
	~D3D() {}
	// [싱글턴 패턴]

public:
	friend Singleton;

	virtual void Initialize()	override;
	virtual void Destroy()		override;

public:

	FORCEINLINE static ID3D11Device*			GetDevice()		{ return device; }
	FORCEINLINE static ID3D11DeviceContext*		GetDC()			{ return deviceContext; }
	FORCEINLINE static IDXGISwapChain*			GetSwapChain()	{ return swapChain; }

	FORCEINLINE static D3DDesc*					GetDesc()		{ return description; }
	FORCEINLINE static float					GetWinWidth()	{ return description->screenWidth; }
	FORCEINLINE static float					GetWinHeight()	{ return description->screenHeight; }
	FORCEINLINE static HWND&					GetHandle()		{ return description->handle; }
	FORCEINLINE static ID3D11RenderTargetView*	GetRTV()		{ return renderTargetView; }

	FORCEINLINE static void					SetDesc(D3DDesc* desc) { description = desc; }

public:
	void SetRenderTarget(ID3D11RenderTargetView* rtv = nullptr, ID3D11DepthStencilView* dsv = nullptr);
	void Clear(ID3D11RenderTargetView* rtv = nullptr, ID3D11DepthStencilView* dsv = nullptr);
	void Present();
	void ResizeScreen(float width, float height);

private:
	void EnumerateAdatpters();										// GPU 어뎁터 정보 받아오기
	bool EnumerateAdatpterOutput(D3DEnumAdapterInfo* adapterInfo);	// GPU 어뎁터 출력 정보 받아오기
	
	void SetGPUInfo();	// GPU 정보 설정 함수

	void CreateSwapChainAndDevice();	// COM Interface 생성

	void CreateBackBuffer(float width, float height);	// 백 버퍼 생성
	void DeleteBackBuffer();							// 백 버퍼 해제

private:
	// 유저 정의
	static D3DDesc*				description;	// 정보 구조체

	// DirectX API
	static ID3D11Device*		device;			// 디바이스
	static ID3D11DeviceContext* deviceContext;	// 디바이스 컨텍스트
	static IDXGISwapChain*		swapChain;		// 스왑체인

	static ID3D11Texture2D*			dsvBackBuffer;			// 백 버퍼
	static ID3D11RenderTargetView*	renderTargetView;	// 렌더 타겟 뷰
	static ID3D11DepthStencilView*	depthStencilView;	// 깊이 스텐실 뷰

	// ID3D11Debug* debugDevice; // 디버그 디바이스 ( 디버그에 사용하나 있다는 것만 알려줄 뿐 사용하진 않을 예정 )

	// GPU
	static	vector<D3DEnumAdapterInfo*> adapterInfoArr;			// 어뎁터 정보 벡터
	static  int							selectedAdapterIndex;	// 선택된 어뎁터 번호

	UINT	gpuMemorysize	= 0;			// 그래픽 카드의 메모리 크기
	wstring gpuDescription	= L"Default";	// 그래픽 카드 이름

	UINT	numerator	= 0;	// 모니터 창 주사율 분자
	UINT	denominator = 1;	// 모니터 창 주사율 분모

	bool	init = false;

};