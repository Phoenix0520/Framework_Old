#include "ObjectManager.h"

ObjectManager::~ObjectManager()
{
	Clear();
}

void ObjectManager::Update()
{
	Matrix V = CONTEXT->GetViewMatrix();
	Matrix P = CONTEXT->GetProjectionMatrix();

	for (auto obj : objects)
	{
		if (obj->IsActive())
			obj->Update(V, P);
	}
}

void ObjectManager::Render()
{
	for (auto obj : objects)
	{
		if (obj->IsActive())
			obj->Render();
	}
}

void ObjectManager::Initialize()
{
}

void ObjectManager::Destroy()
{
	for (auto obj : objects)
	{
		obj->Destroy();
		delete obj;
	}

	objects.clear();
}

GameObject * ObjectManager::CreateObject()
{
	GameObject* obj = new GameObject();
	obj->Initialize();
	objects.push_back(obj);
	
	return obj;
}

void ObjectManager::DestroyObject(GameObject * obj)
{
	obj->SetActive(false);
	obj->Destroy();
}

void ObjectManager::Clear()
{
	for (auto obj : objects)
	{
		delete obj;
	}

	objects.clear();
}
