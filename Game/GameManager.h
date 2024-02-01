#pragma once

#include "GameEngine.h"
#include "GameObject.h"
#include "World.h"
#include <vector>

class Player;

class GameManager  : public GameObject
{
public:
	GameManager()
	{
		player = nullptr;

		indexToRemove = 0;

	};

	~GameManager();

	static GameManager* GetInstance()
	{
		return m_instance = (m_instance != nullptr) ? m_instance : new GameManager();
	}

	void Init() override;

	void Update() override;

	template <typename T>
	void InstantiateProjectile(class Vector2D position, float projectileRange, float projectileSpeed)
	{
		if (&world)
		{
			world.CreateEntity<T>(position, projectileRange, projectileSpeed);
		}
	}

	void CreateLevel();


private:
	static GameManager* m_instance;

	World& world = GameEngine::GetEngine()->GetWorld();

	Player* player;

	int indexToRemove;
};