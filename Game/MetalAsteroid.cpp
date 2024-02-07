#include "MetalAsteroid.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <iostream>
#include <random>

MetalAsteroid::MetalAsteroid()
{

	startPosition = Vector2D(1050, rand() % 750);
	speed = 0.f;
}

MetalAsteroid::~MetalAsteroid()
{

}

void MetalAsteroid::Init()
{
	__super::Init();

	AddComponent<TransformComponent>(startPosition.x, startPosition.y);
	AddComponent<SpriteComponent>("../Assets/graphics/MAster96A.bmp", false, false);
	AddComponent<ColliderComponent>(this, 90, 90);

	SetName("MetalAsteroid");
	SetTag(Tag::Asteroid);

	speed = 1.5f;
	transformComponent = &GetComponent<TransformComponent>();
	spriteComponent = &GetComponent<SpriteComponent>();
}

void MetalAsteroid::Update()
{
	__super::Update();

	spriteComponent->ChangeSrcRect(96, 96);
	spriteComponent->ChangeDstRect(96, 96);

	transformComponent->velocity.x = -1 * speed;

	if (transformComponent->position.x < -100)
	{
		Destroy();
	}
}

void MetalAsteroid::BeginOverlap(Entity* otherEntity)
{
	if (otherEntity->GetTag() == Tag::Player)
	{
		otherEntity->TakeDamage(20.f);
	}
}

void MetalAsteroid::EndOverlap(Entity* otherEntity)
{
}
