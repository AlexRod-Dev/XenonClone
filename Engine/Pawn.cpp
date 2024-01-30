#include "Pawn.h"
#include "GameEngine.h"
#include <iostream>
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Vector2D.h"
#include "Explosion.h"
#include "World.h"

Pawn::Pawn()
{
	maxHp = 0.f;
	hp = maxHp;
}

Pawn::~Pawn()
{

}

void Pawn::Init()
{
	__super::Init();
}

void Pawn::Update()
{
	__super::Update();
}

void Pawn::Destroyed(Entity* entity)
{
	World& world = GameEngine::GetEngine()->GetWorld();
	if (&world)
	{
		world.CreateEntity<Explosion>(entity->GetComponent<TransformComponent>().position);
	}
	entity->Destroy();
}
