#pragma once

#include "Pawn.h"
#include "Vector2D.h"

class TransformComponent;
class b2World;
class b2Body;
class b2Fixture;

class Loner : public Pawn
{
public:
	Loner();
	~Loner();

	virtual void Init();
	virtual void Update();

	virtual void Fire();

	virtual void BeginOverlap(Entity* otherEntity);
	virtual void EndOverlap(Entity* otherEntity);

	virtual void TakeDamage(float damage);

private:
	Vector2D startPosition;
	Vector2D playerPosition;
	float speed;

	TransformComponent* transformComponent = nullptr;

	//Fire Timers
	float fireTimer;
	float fireTimerMax;
};