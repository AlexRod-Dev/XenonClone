#include "Player.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Input.h"
#include "GameManager.h"
#include "PlayerProjectile.h"
#include "ColliderComponent.h"
#include "PlayerProjectileMedium.h"
#include "PlayerProjectileHeavy.h"
#include "LogOutput.h"
#include "Companion.h"

Player::Player()
{
	playerStartPosition = Vector2D(20.f, 350.f);
	playerSpeed = 0.f;
	currentWeaponAugment = WeaponAugment::DEFAULT;

	rightCompanion = nullptr;
	leftCompanion = nullptr;

	fireTimer = 0.f;
	fireTimerMax = 2.5f;
	canFire = true;
	isFiring = false;

	this->maxHp = 100.f;
	this->hp = maxHp;

	playerLives = 3;
	numOfCompanions = 0;
}

Player::~Player()
{
	playerTransform = nullptr;
	spriteComponent = nullptr;
	rightCompanion = nullptr;
	leftCompanion = nullptr;
}

void Player::Init()
{
	__super::Init();

	AddComponent<TransformComponent>(playerStartPosition.x, playerStartPosition.y);
	playerTransform = &GetComponent<TransformComponent>();

	AddComponent<SpriteComponent>("../Assets/graphics/Ship.bmp", true, true);
	spriteComponent = &GetComponent<SpriteComponent>();
	spriteComponent->CreateAnimation("PlayerTurn", 1, 3, 300);
	spriteComponent->CreateAnimation("PlayerIdle", 0, 1, 1000);
	spriteComponent->PlayAnimation("PlayerIdle");

	AddComponent<ColliderComponent>(this, 64, 64);

	playerSpeed = 4.f;

	SetName("Player");

	SetTag(Tag::Player);

	gunOffset = Vector2D(15, 8);

	World& world = GameEngine::GetEngine()->GetWorld();
	if (&world)
	{
		for (int i = 0; i < 2; ++i)
		{
			Companion* companion = world.CreateEntity<Companion>(this);
			playerCompanions.push_back(companion);
		}
	}

	rightCompanion = playerCompanions[0];
	leftCompanion = playerCompanions[1];


	DebugLog(LogMessage::LOG, "Player Initialized");
	DebugLog(LogMessage::WARNING, "Player lives: " + std::to_string(playerLives));
}

void Player::Update()
{
	__super::Update();

	playerPosition = playerTransform->position;

	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W)
		|| Input::GetInstance()->GetButtonDown(SDL_CONTROLLER_BUTTON_DPAD_UP)
		|| Input::GetInstance()->GetAxis(SDL_CONTROLLER_AXIS_LEFTY) < -1)
	{
		spriteComponent->PlayAnimation("PlayerTurn");
		spriteComponent->spriteFlip = SDL_FLIP_HORIZONTAL;
		playerTransform->velocity.y = -1 * playerSpeed;
	}
	else
	{
		spriteComponent->PlayAnimation("PlayerIdle");
		spriteComponent->spriteFlip = SDL_FLIP_NONE;
		playerTransform->velocity.y = 0;
	}

	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A)
		|| Input::GetInstance()->GetButtonDown(SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		|| Input::GetInstance()->GetAxis(SDL_CONTROLLER_AXIS_LEFTX) < 0)
	{
		
		playerTransform->velocity.x = -1 * playerSpeed;
	}
	else
	{
		
		playerTransform->velocity.x = 0;
	}

	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_S)
		|| Input::GetInstance()->GetButtonDown(SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		|| Input::GetInstance()->GetAxis(SDL_CONTROLLER_AXIS_LEFTY) > 0)
	{
		spriteComponent->PlayAnimation("PlayerTurn");
		playerTransform->velocity.y = 1 * playerSpeed;
	}

	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_D)
		|| Input::GetInstance()->GetButtonDown(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		|| Input::GetInstance()->GetAxis(SDL_CONTROLLER_AXIS_LEFTX) > 0)
	{
		
		playerTransform->velocity.x = 1 * playerSpeed;
	}

	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE) || Input::GetInstance()->GetButtonDown(SDL_CONTROLLER_BUTTON_A))
	{
		Fire();
		FireCooldown();
	}
	else if (Input::GetInstance()->GetKeyUp(SDL_SCANCODE_SPACE) || Input::GetInstance()->GetButtonUp(SDL_CONTROLLER_BUTTON_A))
	{
		isFiring = false;
	}
}

void Player::Fire()
{
	if (CanFire())
	{
		if (currentWeaponAugment == WeaponAugment::DEFAULT)
		{
			isFiring = true;
			//DebugLog(LogMessage::LOG, "Fire!");
			GameManager::GetInstance()->InstantiateProjectile<PlayerProjectile>(Vector2D(playerPosition.x + gunOffset.x, playerPosition.y + gunOffset.y * 2), 1000, 12);
		}
		else if (currentWeaponAugment == WeaponAugment::MEDIUM)
		{
			isFiring = true;
			GameManager::GetInstance()->InstantiateProjectile<PlayerProjectileMedium>(Vector2D(playerPosition.x + gunOffset.x, playerPosition.y + gunOffset.y), 1000, 12);
		}
		else if (currentWeaponAugment >= WeaponAugment::HEAVY)
		{
			isFiring = true;
			GameManager::GetInstance()->InstantiateProjectile<PlayerProjectileHeavy>(Vector2D(playerPosition.x + gunOffset.x, playerPosition.y + gunOffset.y * 2), 1000, 12);
		}
	}
	else
	{
		isFiring = false;
	}
}

void Player::BeginOverlap(Entity* otherEntity)
{
	if (otherEntity->GetTag() == Tag::Enemy)
	{
		TakeDamage(5.f);
		GetComponent<SpriteComponent>().SetFlashing(true);
	}

	if (otherEntity->GetTag() == Tag::Asteroid || otherEntity->GetTag() == Tag::EnemyProjectile)
	{
		GetComponent<SpriteComponent>().SetFlashing(true);
	}
}

void Player::EndOverlap(Entity* otherEntity)
{
	GetComponent<SpriteComponent>().SetFlashing(false);
}

bool Player::CanFire()
{
	return canFire;
}

bool Player::IsFiring()
{
	return isFiring;
}

void Player::FireCooldown()
{
	canFire = false;

	fireTimer += 0.3f;

	if (fireTimer >= fireTimerMax)
	{
		canFire = true;
		fireTimer = 0.f;
	}
}

void Player::UpgradeWeapon(WeaponAugment upgrade)
{
	currentWeaponAugment = upgrade;
	DebugLog(LogMessage::WARNING, "Upgraded Player Weapon");
}

void Player::AttachCompanion()
{
	World& world = GameEngine::GetEngine()->GetWorld();
	if (&world)
	{
		world.CreateEntity<Companion>(this);
	}
}

void Player::ResetLife()
{
	this->hp = this->maxHp;
	playerLives--;
	GameManager::GetInstance()->EraseLife();
	DebugLog(LogMessage::WARNING, "Player Lives: " + std::to_string(playerLives));
}

void Player::TakeDamage(float damage)
{
	this->hp -= damage;
	
	DebugLog(LogMessage::WARNING, "Player took " + std::to_string((int) damage) + " damage");
	DebugLog(LogMessage::WARNING, "Player has " + std::to_string((int) this->hp) + " health");

	if (this->hp <= 0)
	{
		if (playerLives > 0)
		{
			ResetLife();
		}
		else if (playerLives <= 0)
		{
			playerLives = 0;

			/*	if (playerCompanion1 || playerCompanion2)
				{
					playerCompanion1->Destroy();
					playerCompanion2->Destroy();
				}*/

			DebugLog(LogMessage::WARNING, "Player died");
			Destroy();
		}
	}
}

void Player::BoostHealth(float health)
{
	if (this->hp < this->maxHp)
	{
		this->hp += health;
		DebugLog(LogMessage::WARNING, "Player received " + std::to_string((int)health) + " health");
	}
	else
	{
		this->hp = this->maxHp;
	}
}

bool Player::IsAlive()
{
	if (this->hp <= 0)
	{
		return false;
	}
	return true;
}
