#pragma once

#include "GameObject.h"
#include "Vector2D.h"

class Projectile : public GameObject
{
public:
	Projectile();
	Projectile(Vector2D position, float projRange, float projSpeed);
	~Projectile();

	virtual void Init();
	virtual void Update();

	virtual void Destroyed(Entity* entity);

	float projectileDamage;
};