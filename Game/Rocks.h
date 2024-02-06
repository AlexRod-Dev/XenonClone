#pragma once

#include "EntityComponentSystem.h"

class SpriteComponent;
class TransformComponent;

class Rocks : public Entity
{
public:
	Rocks(bool isAlive);
	~Rocks();

	virtual void Init();
	virtual void Update();

private:
	float scrollingSpeed = 0.f;
	int horizontalMargin = 0.f;
	int verticalMargin = 0.f;
	int whichRocks = 0;

	SpriteComponent* spriteComponent;
	TransformComponent* transformComponent;

	bool isActive;
};