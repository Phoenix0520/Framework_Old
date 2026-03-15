#include "SceneManager.h"
#include "Scenes/Minecraft.h"

#define CheckSceneEmpty	if (curSceneNum == -1) return	// 씬이 비었는지 체크하는 매크로

void SceneManager::Initialize()
{
	PushScene(new Minecraft());
}

void SceneManager::PushScene(Scene* scene)
{
	sceneArr.push_back(scene);	// 씬을 배열에 집어넣는다

	if (curSceneNum == -1)	// 이 씬이 처음 만들어진 씬이라면
	{
		curSceneNum = 0;									
		ChangeScene(sceneArr[curSceneNum]->GetSceneName());
	}
}

void SceneManager::ChangeScene(wstring name)
{
	CheckSceneEmpty;

	UINT index = 0;

	for (Scene* scene : sceneArr)
	{
		if (scene->GetSceneName() == name) // 입력한 씬과 이름을 비교한다
		{
			if (index == curSceneNum) break;	// 현재 씬 이름을 입력하면 변경하지 않는다

			prevSceneNum = curSceneNum;	// 이전 씬 번호에 현재 씬 번호를 넣고
			curSceneNum = index;		// 현재 씬 번호에 입력한 씬 번호를 넣는다
			
			if (!scene->GetInit())
				scene->Initialize();

			if (scene->GetReset())	// 리셋 기능이 활성화 된경우
				scene->Reset();		// 리셋한다
		}

		index++;
	}
}

void SceneManager::UpdateScene()
{
	CheckSceneEmpty;

	for (Scene* scene : sceneArr)
	{
		if (scene->GetSceneName() == sceneArr[curSceneNum]->GetSceneName()) // 현재 씬을 업데이트 한다 이하 구조도 동일
			scene->Update();
	}
}

void SceneManager::ResizeScreen()
{
	CheckSceneEmpty;

	for (Scene* scene : sceneArr)
	{
		if (scene->GetSceneName() == sceneArr[curSceneNum]->GetSceneName())
			scene->ResizeScreen();
	}
}
