#include "Window.h"
#include "Resources/Resource.h"

bool Window::minimized = false;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpParam, int nCmdShow)
{
	D3DDesc* desc = D3D::GetDesc();
	desc->appName = L"CustomDXEngine";
	desc->instance = hInst;
	desc->fullScreen = false;
	desc->vsync = false;
	desc->handle = NULL;
	desc->screenWidth = 1280;
	desc->screenHeight = 720;
	desc->bgColor = Color(0.3f, 0.3f, 0.3f, 1.0f);
	// D3D 설명 설정

	Window::Create();
	D3D::Create();

	Time::Create();
	Audio::Create();
	Keyboard::Create();
	Mouse::Create();

	Context::Create();
	Gui::Create();
	TextureManager::Create();
	ShaderManager::Create();
	Renderer::Create();
	SceneManager::Create();

	// 필요한것들 순서대로 생성

	MSG msg;

	while (true)	// 메세지 처리문
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // 메세지를 받을때까지 기다리는 GetMessage 대신 PeekMessage 사용
		{
			if (msg.message == WM_QUIT)	break; // 받은 메세지가 WM_QUIT (종료 메세지) 일 시 반복문 종료

			TranslateMessage(&msg);	// 메세지를 번역 : 입력을 메세지로 바꿈
			DispatchMessage(&msg);	// 메세지를 변환 : 이중 키 등
		}
		else // 입력 메세지가 없으면
		{
			if (!Window::IsMinimized())
				WINDOW->MainRender();	// 창 내용 진행
		}
	}

	SceneManager::Delete();
	Renderer::Delete();
	ShaderManager::Delete();
	TextureManager::Delete();
	Gui::Delete();
	Context::Delete();

	Mouse::Delete();
	Keyboard::Delete();
	Audio::Delete();
	Time::Delete();

	D3D::Delete();
	Window::Delete();
	// 사용이 끝났으므로 전부 해제
	
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MOUSE->MouseProc(uMsg, wParam, lParam);

	if (GUI->GuiProc(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
		{
			Window::SetMinimized(true);
		}
		else
		{
			Window::SetMinimized(false);

			float width = (float)LOWORD(lParam);
			float height = (float)HIWORD(lParam);

			if (D3DSYSTEM != nullptr)
				D3DSYSTEM->ResizeScreen(width, height);

			if (CONTEXT != nullptr)
				CONTEXT->ResizeScreen();

			if (SCENEMANAGER != nullptr)
				SCENEMANAGER->ResizeScreen();

			if (RENDERER != nullptr)
				RENDERER->ResizeScreen((UINT)width, (UINT)height);
		}
	}
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Window::Initialize()
{
	// 창 초기화 : 아래는 Windows API 에 관한 내용이므로 딱히 설명하지 않겠다 궁금한점은 인터넷에 찾아보기 바란다

	D3DDesc* desc = D3D::GetDesc();

	WNDCLASSEX wndClass;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon			= LoadIcon(NULL, MAKEINTRESOURCE(IDI_BIGICO));
	wndClass.hIconSm		= LoadIcon(NULL, MAKEINTRESOURCE(IDI_SMALLICO));
	wndClass.hInstance		= desc->instance;
	wndClass.lpfnWndProc	= (WNDPROC)WndProc;
	wndClass.lpszClassName	= desc->appName.c_str();
	wndClass.lpszMenuName	= NULL;
	wndClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize			= sizeof(WNDCLASSEX);

	WORD wHr = RegisterClassEx(&wndClass);
	assert(wHr != 0);

	if (desc->fullScreen == true)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize			= sizeof(DEVMODE);
		devMode.dmPelsWidth		= (DWORD)desc->screenWidth;
		devMode.dmPelsHeight	= (DWORD)desc->screenHeight;
		devMode.dmBitsPerPel	= 32;
		devMode.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	desc->handle = CreateWindowEx
	(
		WS_EX_APPWINDOW, 
		desc->appName.c_str(),
		desc->appName.c_str(),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
		0,
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		desc->instance,
		NULL
	);

	Check(desc->handle);

	D3D::SetDesc(desc);

	RECT rect = { 0, 0, (LONG)desc->screenWidth, (LONG)desc->screenHeight };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc->screenWidth) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc->screenHeight) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		desc->handle,
		centerX,
		centerY,
		rect.right - rect.left,
		rect.bottom - rect.top,
		TRUE
	);
	ShowWindow(desc->handle, SW_SHOWNORMAL);
	SetForegroundWindow(desc->handle);
	SetFocus(desc->handle);

	ShowCursor(true);
}

void Window::Destroy()
{
	D3DDesc* desc = D3D::GetDesc();

	if (desc->fullScreen == true)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(desc->handle);

	UnregisterClass(desc->appName.c_str(), desc->instance);
}

void Window::MainRender()
{
	TIME->Update();
	AUDIO->Update();

	if (ImGui::IsAnyItemHovered() == false)
	{
		KEYBOARD->Update();
		MOUSE->Update();
	}

	CONTEXT->Update();
	GUI->Update();

	SCENEMANAGER->UpdateScene();

	RENDERER->PreRender();
	RENDERER->Render();
	RENDERER->PostRender();

	CONTEXT->Render();
	GUI->Render();
	
	D3DSYSTEM->Present();
}