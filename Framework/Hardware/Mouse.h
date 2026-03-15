#pragma once

#include "Framework.h"
#include "Base/Singleton.h"

#define MAX_MOUSE 8

enum class ButtonState
{
	BUTTON_NONE = 0,
	BUTTON_DOWN,
	BUTTON_UP,
	BUTTON_PRESS,
	BUTTON_DBLCLK
};

class Mouse : public Singleton<Mouse>
{
private:
	Mouse() { }
	Mouse(const Mouse& other) {}
	~Mouse() {}

public:
	friend Singleton<Mouse>;

	virtual void Initialize()	override;
	virtual void Update()		override;

public:
	LRESULT MouseProc(UINT message, WPARAM wParam, LPARAM lParam);
	FORCEINLINE void SetHandle(HWND hWnd) { handle = hWnd; }

public:
	FORCEINLINE Vector3 GetPosition()		const { return position; }
	FORCEINLINE Vector3 GetOldPosition()	const { return oldPos; }
	FORCEINLINE Vector3 GetWheelMoveValue() const { return wheelMoveValue; }
	FORCEINLINE Vector3 GetDelta() const		  { return delta; }

	FORCEINLINE void SetPosition(Vector3 pos)				{ position = pos; }
	FORCEINLINE void SetPosition(float x, float y, float z) { position = Vector3(x, y, z); }
	FORCEINLINE void SetWheelMoveValue(float val)			{ wheelOldStatus.z = wheelStatus.z; wheelStatus.z += val; }

	FORCEINLINE void SetMousePointLock(bool val) { pointLock = val; }

	void GetMouseRay(Vector3& outOrigin, Vector3& outDir);

public:	// 키 
	bool Down(DWORD button)		{ return buttonMap[button] == (BYTE)ButtonState::BUTTON_DOWN; }
	bool Up(DWORD button)		{ return buttonMap[button] == (BYTE)ButtonState::BUTTON_UP; }
	bool Press(DWORD button)	{ return buttonMap[button] == (BYTE)ButtonState::BUTTON_PRESS; }


private:
	HWND handle;
	Vector3 position; //마우스 위치
	Vector3 oldPos;
	Vector3 delta;

	BYTE buttonCurStatus[MAX_MOUSE];
	BYTE buttonOldStatus[MAX_MOUSE];
	BYTE buttonMap		[MAX_MOUSE];

	Vector3 wheelStatus;
	Vector3 wheelOldStatus;
	Vector3 wheelMoveValue;

	DWORD timeDblClk;
	DWORD startDblClk[MAX_MOUSE];
	UINT  buttonCount[MAX_MOUSE];

	bool pointLock = false;

};

