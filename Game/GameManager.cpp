#include "GameManager.h"
#include "Level.h"
#include "EntityComponentSystem.h"
#include "World.h"
#include "Level.h"
#include "Player.h"
#include "Loner.h"
#include "Rusher.h"
#include "WeaponPickup.h"
#include "ShieldPickup.h"
#include "BigAsteroid.h"
#include "MetalAsteroid.h"
#include "Rocks.h"

GameManager* GameManager::m_instance = nullptr;

GameManager::~GameManager()
{
	delete m_instance;
}

void GameManager::Init()
{
	CreateLevel();
}

void GameManager::Update()
{
	SpawnRocks();
	SpawnEnemies();
	SpawnPickups();
	SpawnAsteroids();


}

void GameManager::CreateLevel()
{
	if(&world)
	{
		world.CreateEntity<Level>();

		player = world.CreateEntity<Player>();

	}
}

void GameManager::SpawnEnemies()
{
	lonerSpawnTimer += 0.5f;
	rusherSpawnTimer += +0.5f;


	if(lonerSpawnTimer >= lonerSpawnTimerMax)
	{
		enemiesToSpawn = rand() % 2 + 1;
		for(int i = 0; i < enemiesToSpawn; ++i)
		{
			world.CreateEntity<Loner>();
		}

		lonerSpawnTimer = 0.f;
	}

	if (rusherSpawnTimer >= rusherSpawnTimerMax)
	{
		enemiesToSpawn = rand() % 5 + 3;

		for (int i = 0; i < enemiesToSpawn; ++i)
		{
			world.CreateEntity<Rusher>();
		}

		rusherSpawnTimer = 0.f;
	}
}

void GameManager::SpawnPickups()
{
	pickupSpawnTimer += 0.5f;

	if(pickupSpawnTimer >= pickupSpawnTimerMax)
	{
		pickupsToSpawn = 1;

		for(int i = 0; i < pickupsToSpawn; ++i)
		{
			world.CreateEntity<WeaponPickup>();
			world.CreateEntity<ShieldPickup>();
		}

		pickupSpawnTimer = 0.f;
	}


}

void GameManager::SpawnAsteroids()
{
	asteroidSpawnTimer += 0.5f;

	if(asteroidSpawnTimer >= asteroidSpawnTimerMax)
	{
		asteroidsToSpawn = rand() % 1 + 2;

		for(int i = 0; i < asteroidsToSpawn; ++i)
		{
			world.CreateEntity<BigAsteroid>();
			world.CreateEntity<MetalAsteroid>();
		}

		asteroidSpawnTimer = 0.f;
	}
}

void GameManager::SpawnRocks()
{
	rocksSpawnTimer += 0.5f;

	if(rocksSpawnTimer >= rocksSpawnTimerMax)
	{
		world.CreateEntity<Rocks>(true);
		rocksSpawnTimer = 150 + (rand() % (200 - 150) + 1);
		rocksSpawnTimer = 0.f;

	}
}
