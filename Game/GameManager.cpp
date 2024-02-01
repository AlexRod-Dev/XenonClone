#include "GameManager.h"
#include "Level.h"
#include "EntityComponentSystem.h"
#include "World.h"
#include "Level.h"
#include "Player.h"

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
//
}

void GameManager::CreateLevel()
{
	if(&world)
	{
		world.CreateEntity<Level>();

		player = world.CreateEntity<Player>();

	}
}
