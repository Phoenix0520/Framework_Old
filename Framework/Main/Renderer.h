#pragma once

#include "Framework.h"

class Scene;
class GBuffer;
class LightingPass;

class Renderer : public Singleton<Renderer>
{
private:
	Renderer() = default;
	~Renderer() = default;

public:
	friend Singleton;

public:
	virtual void Initialize() override;
	virtual void Destroy() override;

	virtual void Update() override NoDefine

	void PreRender();
	virtual void Render() override;
	void PostRender();

public:
	void ResizeScreen(UINT width, UINT height);

private:
	void RenderGeometryPass(Scene* scene);
	void RenderLightingPass();

	void UpdateViewport();
	void ClearViewport();

private:
	unique_ptr<GBuffer>			gBuffer;
	unique_ptr<LightingPass>	lightingPass;

};