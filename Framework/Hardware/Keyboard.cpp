#include "Keyboard.h"

void Keyboard::Initialize()
{
	ZeroMemory(keyState,	sizeof(keyState));
	ZeroMemory(keyOldState, sizeof(keyOldState));
	ZeroMemory(keyMap,		sizeof(keyMap));
}

void Keyboard::Update()
{
	memcpy(keyOldState, keyState, sizeof(keyOldState));

	ZeroMemory(keyState,	sizeof(keyState));
	ZeroMemory(keyMap,		sizeof(keyMap));

	BOOL retval = GetKeyboardState(keyState);

	for (DWORD i = 0; i < KEY_MAX; i++)
	{
		BYTE key = keyState[i] & 0x80;	// ASCII 코드일시 false 아닐시 true
		keyState[i] = key ? 1 : 0;		// 키가 눌렸다면 1 아닐시 0

		int oldState = keyOldState[i];
		int curState = keyState[i];

		if (oldState == 0 && curState == 1)
			keyMap[i] = (BYTE)KeyState::KEY_DOWN;	// 이전 0, 현재 1 - KeyDown

		else if (oldState == 1 && curState == 0)
			keyMap[i] = (BYTE)KeyState::KEY_UP;		// 이전 1, 현재 0 - KeyUp

		else if (oldState == 1 && curState == 1)
			keyMap[i] = (BYTE)KeyState::KEY_PRESS;	// 이전 1, 현재 1 - KeyPress

		else
			keyMap[i] = (BYTE)KeyState::KEY_NONE;
	}
}
