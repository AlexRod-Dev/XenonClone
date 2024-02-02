#include "SmallAsteroid.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

SmallAsteroid::SmallAsteroid()
{
	startPosition = Vector2D(1050, rand() % 750);
	speed = 0.f;
	maxHP = 60.f;
	hp = maxHP;
}

SmallAsteroid::~SmallAsteroid()
{

}

void SmallAsteroid::Init()
{
	__super::Init();

	AddComponent<TransformComponent>(startPosition.x, startPosition.y);
	AddComponent<SpriteComponent>("../Assets/graphics/SAster32A.bmp", false, false);
	AddComponent<ColliderComponent>(this, 32, 32);

	SetName("SmallAsteroid");
	SetTag(Tag::Asteroid);

	speed = 2.5f;
	transformComponent = &GetComponent<TransformComponent>();
}

void SmallAsteroid::Update()
{
	__super::Update();

	transformComponent->velocity.x = -1 * speed;

	if (transformComponent->position.x > 1000 || transformComponent->position.y < -50)
	{
		Destroy();
	}
}

void SmallAsteroid::Break()
{
	Destroyed(this);
	this->m_destroyed = true;
}

void SmallAsteroid::TakeDamage(float damage)
{
	hp -= damage;

	if (this->hp <= 0)
	{
		Break();
	}
}

void SmallAsteroid::BeginOverlap(Entity* otherEntity)
{
	if (otherEntity->GetTag() == Tag::Player)
	{
		otherEntity->TakeDamage(10.f);
	}

	if (otherEntity->GetTag() == Tag::Projectile)
	{
		GetComponent<SpriteComponent>().SetFlashing(true);
	}
}

void SmallAsteroid::EndOverlap(Entity* otherEntity)
{
	if (otherEntity->GetTag() == Tag::Projectile)
	{
		GetComponent<SpriteComponent>().SetFlashing(false);
	}
}
