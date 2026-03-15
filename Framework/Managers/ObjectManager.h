#pragma once
#include "Framework.h"
#include "GameObject.h"

class GameObject;

class ObjectManager : public Singleton<ObjectManager>
{
private:
	ObjectManager() {}
	ObjectManager(const ObjectManager& other);
	~ObjectManager();

public:
	friend Singleton;

	virtual void Update() override;
	virtual void Render() override;
	virtual void Initialize() override;
	virtual void Destroy() override;

	GameObject* CreateObject();

	void DestroyObject(GameObject* obj);

	void Clear();
	
public:


private:
	vector<GameObject*> objects;

	
};