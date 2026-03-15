#pragma once

#include "Framework.h"
#include "Base/Scene.h"
#include "GameObject/Block.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"

#include "Software/FBXImporter.h"
#include "Meshes/Skeletal/SkeletalMesh.h"

#include "GameObject/Chara.h"

class SkeletalMesh;
class DirectionalLight;
class PointLight;
class SpotLight;
class Chara;

class Minecraft : public Scene
{
public:
	Minecraft() { Initialize(); }
	~Minecraft() { Destroy(); }

public:
	virtual void Initialize();
	virtual void Destroy();
	
	virtual void Reset();
	
	virtual void Update();
	virtual void PreRender()	NoDefine;
	virtual void Render();
	virtual void RenderGBuffer(GBuffer* gBuffer);
	virtual void PostRender()	NoDefine;

	virtual void ResizeScreen() NoDefine;

	void ReadMap(wstring filePath);
	void WriteMap(wstring filePath);

private:
	vector<int> initMap;

	vector<Block*> blocks;

	Block* focusedBlock = nullptr;
	DirectionalLight* dLight = nullptr;
	PointLight* pLight = nullptr;
	SpotLight* sLight = nullptr;

	Chara* chara = nullptr;

};