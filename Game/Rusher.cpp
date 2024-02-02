#include "Rusher.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <ColliderComponent.h>

Rusher::Rusher()
{
	
	startPosition = Vector2D(((rand() > RAND_MAX / 2) ? -50 : 1050), rand() % 750) ;
	speed = 0.f;

	this->maxHp = 100.f;
	this->hp = this->maxHp;
}

Rusher::~Rusher()
{

}

void Rusher::Init()
{
	__super::Init();

	AddComponent<TransformComponent>(startPosition.x, startPosition.y);
	AddComponent<SpriteComponent>("../Assets/graphics/rusherB.bmp", true, true);
	AddComponent<ColliderComponent>(this, 64, 64);
	GetComponent<SpriteComponent>().CreateAnimation("EnemyIdle", 0, 4, 200);
	GetComponent<SpriteComponent>().PlayAnimation("EnemyIdle");

	SetName("Rusher");
	SetTag(Tag::Enemy);

	speed = rand() % 3 + (int)2.8f;
	transformComponent = &GetComponent<TransformComponent>();
}

void Rusher::Update()
{
	__super::Update();

	if (startPosition.y == 1000)
	{
		transformComponent->velocity.x = -1 * speed;
	}
	else
	{
		transformComponent->velocity.x = 1 * speed;
	}


	if (transformComponent->position.x < -100 || transformComponent->position.x >1200)
	{
		Destroyed(this);
	}
}

void Rusher::BeginOverlap(Entity* otherEntity)
{
	if (otherEntity->GetTag() == Tag::Projectile)
	{
		GetComponent<SpriteComponent>().SetFlashing(true);
	}
}

void Rusher::EndOverlap(Entity* otherEntity)
{
	if (otherEntity->GetTag() == Tag::Projectile)
	{
		GetComponent<SpriteComponent>().SetFlashing(false);
	}
}

void Rusher::Fire()
{
}

void Rusher::TakeDamage(float damage)
{
	this->hp -= damage;

	if (this->hp <= 0)
	{
		Destroyed(this);
	}
}
