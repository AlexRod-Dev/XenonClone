#pragma once

#include "EntityComponentSystem.h"

class GameObject : public Entity
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init() = 0;
	virtual void Update() = 0;

	virtual void Destroyed(Entity* entity);
};