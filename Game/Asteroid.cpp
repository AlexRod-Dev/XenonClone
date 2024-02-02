#include "Asteroid.h"
#include "GameManager.h"
#include "Explosion.h"
#include "TransformComponent.h"
#include "World.h"

Asteroid::Asteroid()
{
}

Asteroid::~Asteroid()
{
}

void Asteroid::Init()
{
	__super::Init();
}

void Asteroid::Update()
{
	__super::Update();
}

void Asteroid::Break()
{
	Destroy();
}

void Asteroid::Destroyed(Entity* entity)
{
	World& world = GameEngine::GetEngine()->GetWorld();
	if (&world)
	{
		world.CreateEntity<Explosion>(entity->GetComponent<TransformComponent>().position);
	}
	entity->Destroy();
}
