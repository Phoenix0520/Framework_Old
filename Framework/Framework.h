#pragma once

// Debug 모드시
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")	// 디버그 콘솔
#endif // _DEBUG

// Window
#include <Windows.h>
#include <commdlg.h>
#include <assert.h>

// STL
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <functional>
#include <iterator>
#include <thread>
#include <mutex>
using namespace std;

// Direct3D
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <d3dx11effect.h>
#include <DirectXTex.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Effects11d.lib")
#pragma comment(lib, "directxtex.lib")

// ImGui
#include "ImGui/imconfig.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#pragma comment(lib,"ImGui/imGui.lib")

// FMOD
#include "FMOD/fmod.hpp"
#pragma comment(lib, "FMOD/fmodex_vc.lib")

// 유저 정의
#define Check(hr)	{ assert(SUCCEEDED(hr)); }
#define Super		__super
#define NoDefine	{ ; }	// 현재 클래스에서는 이 함수를 정의하지 않을 때 사용

#define SafeRelease(p)		{ if(p){ (p)->Release(); (p) = NULL; } }
#define SafeDelete(p)		{ if(p){ delete (p); (p) = NULL; } }
#define SafeDeleteArray(p)	{ if(p){ delete [] (p); (p) = NULL; } }

#define SAFE_RELEASE(p)		{ if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p)		{ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETEARRAY(p)	{ if(p){ delete [] (p); (p) = NULL; } }

// 타입 재정의
typedef D3DXVECTOR2		Vector2;
typedef D3DXVECTOR3		Vector3;
typedef D3DXVECTOR4		Vector4;
typedef D3DXCOLOR		Color;
typedef D3DXMATRIX		Matrix;
typedef D3DXQUATERNION	Quaternion;
typedef D3DXPLANE		Plane;

// 공용 헤더
#include "Main/D3D.h"
#include "Main/Window.h"
#include "Main/Renderer.h"

#include "Hardware/Time.h"
#include "Hardware/Audio.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Mouse.h"


#include "Base/VertexLayouts.h"
#include "Base/Transform.h"
#include "Base/ConstantBuffers.h"

#include "Utilities/BinaryFile.h"
#include "Utilities/Math.h"
#include "Utilities/Path.h"
#include "Utilities/String.h"
#include "Utilities/Xml.h"

#include "Viewers/Camera.h"
#include "Viewers/Projection.h"
#include "Viewers/Viewport.h"
#include "Viewers/RenderTarget.h"
#include "Viewers/DepthStencil.h"
#include "Viewers/GBuffer.h"
#include "Viewers/LightingPass.h"
#include "Viewers/Context.h"

#include "Renders/Shader.h"
#include "Renders/Texture.h"
#include "Renders/Material.h"

#include "Software/MeshBuilder.h"
#include "Software/Gui.h"

#include "Managers/SceneManager.h"
#include "Managers/TextureManager.h"
#include "Managers/ShaderManager.h"
#include "Managers/ObjectManager.h"

// 공용 매크로
#define D3DSYSTEM			D3D::GetInstance()
#define WINDOW				Window::GetInstance()
#define RENDERER			Renderer::GetInstance()

#define SCENEMANAGER		SceneManager::GetInstance()
#define GUI					Gui::GetInstance()
#define CONTEXT				Context::GetInstance()
#define TEXTUREMANAGER		TextureManager::GetInstance()
#define SHADERMANAGER		ShaderManager::GetInstance()
#define OBJMANAGER			ObjectManager::GetInstance()

#define TIME				Time::GetInstance()
#define DELTA				TIME->GetDeltaTime()
#define STARTLOCKFPS		144.0f
#define TIMERAUTODESTROY	true

#define AUDIO							Audio::GetInstance()
#define ADDSOUND(name, file, repeat)	audio->AddSound(name, file, repeat)
#define PLAYSOUND(name, vol)			audio->Play(name, vol)
#define ISPLAYING(name)					audio->Playing(name)
#define STOPSOUND(name)					audio->Stop(name)
#define PAUSESOUND(name)				audio->Pause(name)
#define RESUMESOUND(name)				audio->Resume(name)

#define KEYBOARD			Keyboard::GetInstance()
#define KEYDOWN(key)		KEYBOARD->Down(key)
#define KEYUP(key)			KEYBOARD->Up(key)
#define KEYPRESS(key)		KEYBOARD->Press(key)

#define MOUSE				Mouse::GetInstance()
#define MBUTTON_L			0
#define MBUTTON_R			1
#define MBUTTON_M			2
#define MOUSEDOWN(button)	MOUSE->Down(button)
#define MOUSEUP(button)		MOUSE->Up(button)
#define MOUSEPRESS(button)	MOUSE->Press(button)

#define SHADER_NONE			0
#define SHADER_COVER		1
#define SHADER_CLIPMASK		2
#define SHADER_BLEND		3
#define SHADER_ADD			4
#define SHADER_GRAYSCALE    5
#define SHADER_INVERT		6
#define SHADER_HIGHLIGHT	7
#define SHADER_COLORTINT	8
#define SHADER_BRIGHTNESS	9
#define SHADER_CONTRAST		10
#define SHADER_GLOW			11
#define SHADER_DAMAGEFLASH	12

#define PrintVector2(vec) printf("%f, %f\n", vec.x, vec.y)
#define PrintVector3(vec) printf("%f, %f, %f\n", vec.x, vec.y, vec.z)

#define MulVec3(v1, v2) Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z)