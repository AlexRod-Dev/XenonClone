#pragma once

#include "GameObject.h"
#include "Vector2D.h"

class Explosion : public GameObject
{
public:
	Explosion(Vector2D position);
	~Explosion();

	virtual void Init();
	virtual void Update();

private:
	Vector2D startPosition;

	float explosionTimer;
};