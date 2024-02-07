#include "BigAsteroid.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "LogOutput.h"
#include "GameManager.h"
#include "World.h"
#include "Vector2D.h"
#include "MediumAsteroid.h"
#include <iostream>
#include <random>

BigAsteroid::BigAsteroid()
{

	startPosition = Vector2D(1050, rand() % 750 );
	speed = 0.f;
	maxHP = 150.f;
	hp = maxHP;
}

BigAsteroid::~BigAsteroid()
{
}

void BigAsteroid::Init()
{
	__super::Init();

	AddComponent<TransformComponent>(startPosition.x, startPosition.y);
	AddComponent<SpriteComponent>("../Assets/graphics/SAster96A.bmp", false, false);
	AddComponent<ColliderComponent>(this, 96, 96);

	SetName("BigAsteroid");
	SetTag(Tag::Asteroid);

	speed = 1.5f;
	transformComponent = &GetComponent<TransformComponent>();
	spriteComponent = &GetComponent<SpriteComponent>();
}

void BigAsteroid::Update()
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

void BigAsteroid::Break()
{
	Destroyed(this);
	this->m_destroyed = true;
}

void BigAsteroid::TakeDamage(float damage)
{
	hp -= damage;

	if (this->hp <= 0)
	{
		World& world = GameEngine::GetEngine()->GetWorld();
		if (&world)
		{
			world.QueueAction([&]() { CreateNewAsteroids(); });
			Break();
		}
	}
}

void BigAsteroid::CreateNewAsteroids()
{
	// Seed the random number generator with the current time
	std::srand(std::time(nullptr));

	// Generate three random integers between -1 and 1 (inclusive)
	int xDirection1 = std::rand() % 3 - 2;
	int xDirection2 = std::rand() % 3 - 2;
	int xDirection3 = std::rand() % 3 - 2;

	World& world = GameEngine::GetEngine()->GetWorld();
	if (&world)
	{
		MediumAsteroid* asteroid1 = world.CreateEntity<MediumAsteroid>();
		asteroid1->GetComponent<TransformComponent>().position = transformComponent->position;
		asteroid1->GetComponent<TransformComponent>().velocity.x = std::rand() % 5 - 4; 
		asteroid1->GetComponent<TransformComponent>().velocity.y = xDirection1;

		MediumAsteroid* asteroid2 = world.CreateEntity<MediumAsteroid>();
		asteroid2->GetComponent<TransformComponent>().position = transformComponent->position;
		asteroid2->GetComponent<TransformComponent>().velocity.x = std::rand() % 5 - 4;
		asteroid2->GetComponent<TransformComponent>().velocity.y = xDirection2; 

		MediumAsteroid* asteroid3 = world.CreateEntity<MediumAsteroid>();
		asteroid3->GetComponent<TransformComponent>().position = transformComponent->position;
		asteroid3->GetComponent<TransformComponent>().velocity.x = std::rand() % 5 - 4;
		asteroid3->GetComponent<TransformComponent>().velocity.y = xDirection3;
	}
}

void BigAsteroid::BeginOverlap(Entity* otherEntity)
{
	if (otherEntity->GetTag() == Tag::Player)
	{
		otherEntity->TakeDamage(18.f);
	}

	if (otherEntity->GetTag() == Tag::Projectile)
	{
		GetComponent<SpriteComponent>().SetFlashing(true);
	}
}

void BigAsteroid::EndOverlap(Entity* otherEntity)
{
	if (otherEntity->GetTag() == Tag::Projectile)
	{
		GetComponent<SpriteComponent>().SetFlashing(false);
	}
}
