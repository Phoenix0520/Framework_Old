#pragma once

#include "Framework.h"
#include "Base/Singleton.h"

#define KEY_MAX	256

enum class KeyState
{
	KEY_NONE = 0,
	KEY_DOWN,
	KEY_UP,
	KEY_PRESS,
};

class Keyboard : public Singleton<Keyboard>
{
private:
	Keyboard() {}
	Keyboard(const Keyboard& other) {}
	~Keyboard() {}

public:
	friend Singleton<Keyboard>;

public:
	virtual void Initialize()	override;
	virtual void Update()		override;

	bool Down(DWORD key)	{ return keyMap[key] == (BYTE)KeyState::KEY_DOWN; }
	bool Up(DWORD key)		{ return keyMap[key] == (BYTE)KeyState::KEY_UP; }
	bool Press(DWORD key)	{ return keyMap[key] == (BYTE)KeyState::KEY_PRESS; }

private:
	BYTE keyState	[KEY_MAX];
	BYTE keyOldState[KEY_MAX];
	BYTE keyMap		[KEY_MAX];

};