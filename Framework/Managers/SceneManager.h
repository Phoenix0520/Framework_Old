#pragma once

#include "Framework.h"
#include "Base/Singleton.h"
#include "Base/Scene.h"

class Scene;

class SceneManager : public Singleton<SceneManager>
{
private:
	SceneManager() {}
	SceneManager(const SceneManager& other);
	~SceneManager() {}

public:
	friend Singleton;

	virtual void Initialize() override;

public:
	void PushScene(Scene* scene);
	void ChangeScene(wstring name);

	void UpdateScene();

	void ResizeScreen();

public:
	FORCEINLINE wstring GetCurSceneName()	const { return sceneArr[curSceneNum]->GetSceneName(); }
	FORCEINLINE wstring GetPrevSceneName()	const { return sceneArr[prevSceneNum]->GetSceneName(); }

	FORCEINLINE Scene* GetCurScene() { return sceneArr[curSceneNum]; }
	FORCEINLINE Scene* GetPrevScene() { return sceneArr[prevSceneNum]; }

private:
	vector<Scene*> sceneArr;

	UINT curSceneNum	= -1;
	UINT prevSceneNum	= -1;

};