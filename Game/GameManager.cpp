#include "GameManager.h"
#include "Level.h"
#include "EntityComponentSystem.h"
#include "World.h"
#include "Level.h"
#include "Player.h"
#include "Loner.h"
#include "Rusher.h"

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

	SpawnEnemies();

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
