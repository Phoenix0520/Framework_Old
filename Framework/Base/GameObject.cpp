#include "GameObject.h"

GameObject::GameObject()
{
	transform = new Transform();
}

GameObject::~GameObject()
{
	SafeDelete(transform);
}

void GameObject::Update(Matrix V, Matrix P)
{
}

void GameObject::Render()
{
}

void GameObject::Initialize()
{
}

void GameObject::Destroy()
{
}
