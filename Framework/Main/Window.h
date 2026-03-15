#pragma once

#include "Framework.h"
#include "Base/Singleton.h"

class Window : public Singleton<Window>
{
private:
	Window() {}
	Window(const Window& other) {}
	~Window() {}

public:
	friend Singleton;

public:
	virtual void Initialize()	override;
	virtual void Destroy()		override;

	void MainRender();

public:
	static bool IsMinimized() { return minimized; }
	static void SetMinimized(bool val) { minimized = val; }

protected:
	static bool minimized;

};