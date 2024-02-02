#include "ShieldPickup.h"
#include "GameManager.h"
#include "Companion.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "LogOutput.h"
#include "Player.h"

ShieldPickup::ShieldPickup()
{
	startPosition = Vector2D(1050, rand() % 750);
	speed = 0.f;
	healthBoost = 5.f;
}

ShieldPickup::~ShieldPickup()
{
}

void ShieldPickup::Init()
{
	__super::Init();

	AddComponent<TransformComponent>(startPosition.x, startPosition.y);
	AddComponent<SpriteComponent>("../Assets/graphics/ShieldPickup.bmp", true, true);
	GetComponent<SpriteComponent>().CreateAnimation("PickupIdle", 0, 8, 150);
	GetComponent<SpriteComponent>().PlayAnimation("PickupIdle");
	AddComponent<ColliderComponent>(this, 64, 64);

	SetName("WeaponPickup");
	SetTag(Tag::Pickup);

	speed = 2.5f;
	transformComponent = &GetComponent<TransformComponent>();
}

void ShieldPickup::Update()
{
	__super::Update();

	transformComponent->velocity.x = -1 * speed;

	if (transformComponent->position.y > 1000)
	{
		Destroy();
	}
}

void ShieldPickup::BeginOverlap(Entity* otherEntity)
{
	if (otherEntity->GetTag() == Tag::Player)
	{
		Player* player = dynamic_cast<Player*>(otherEntity);
		Companion* companion = dynamic_cast<Companion*>(otherEntity);

		if (player)
		{
			player->BoostHealth(healthBoost);
		}

		if (companion)
		{
			companion->BoostHealth(healthBoost);
		}
		Destroy();
	}
}

void ShieldPickup::EndOverlap(Entity* otherEntity)
{
}
