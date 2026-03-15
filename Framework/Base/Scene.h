#pragma once

#include "Framework.h"

class GBuffer;

class Scene
{
public:
	Scene() { }
	Scene(const Scene& other) {}
	virtual ~Scene() { }

public:
	virtual void Initialize()	= 0;	// ¾À ÃÊ±âÈ­
	virtual void Destroy()		= 0;	// ¾À ÇØÁ¦

	virtual void Reset()		= 0;	// ¾À ¸®¼Â

	virtual void Update()							= 0;	// ¾À ¾÷µ¥ÀÌÆ®
	virtual void PreRender()						= 0;	// ¾À ·»´õ Àü ÇØ¾ßÇÒ °Í
	virtual void Render()							= 0;	// ¾À ·»´õ
	virtual void RenderGBuffer(GBuffer* gBuffer)	= 0;
	virtual void PostRender()						= 0;	// ¾À ·»´õ ÈÄ ÇØ¾ßÇÒ °Í

	virtual void ResizeScreen() = 0;

public:
	FORCEINLINE wstring GetSceneName()	const { return sceneName; }
	FORCEINLINE bool GetInit()			const { return init; }
	FORCEINLINE bool GetReset()			const { return reset; }

	FORCEINLINE void SetReset(bool val) { reset = val; }

protected:
	wstring sceneName = L"DefaultScene";	// ¾À ÀÌ¸§
	bool init	= false;					// ¾ÀÀÌ ÃÊ±âÈ­ µÇ¾ú´ÂÁö
	bool reset	= true;						// ¾ÀÀ» ¸®¼ÂÇÒ°ÍÀÎÁö


};