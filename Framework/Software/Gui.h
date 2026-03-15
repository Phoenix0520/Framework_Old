#pragma once

#include "Framework.h"

struct GuiText
{
	D3DXVECTOR2 Position;
	D3DXCOLOR Color;
	string Content;
};

class Gui : public Singleton<Gui>
{
private:
	Gui() {}
	Gui(const Gui& other) {}
	~Gui() {}

public:
	friend Singleton;

	virtual void Initialize()	override;
	virtual void Destroy()		override;
	virtual void Update()		override;
	virtual void Render()		override;

public:
	LRESULT GuiProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void Resize();

	void AddWidget(class Widget* widget);

	void RenderText(GuiText& text);
	void RenderText(float x, float y, string content);
	void RenderText(float x, float y, float r, float g, float b, string content);
	void RenderText(D3DXVECTOR2 position, D3DXCOLOR color, string content);

private:
	void ApplyStyle();

private:
	vector<class Widget*> widgets;
	vector<GuiText> texts;
};