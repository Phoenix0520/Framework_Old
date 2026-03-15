#pragma once

#include "Framework.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

public:
	virtual void Initialize();
	virtual void Destroy();
	
	virtual void Update(Matrix V, Matrix P);
	virtual void Render();


public:
	FORCEINLINE Transform* GetTransform() { return transform; }
	FORCEINLINE bool IsActive() { return active; }

	FORCEINLINE void SetActive(bool val) { active = false; }


protected:
	Transform* transform = nullptr;;
	
	bool active = true;


};