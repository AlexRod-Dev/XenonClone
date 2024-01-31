#pragma once

#include "GameEngine.h"
#include "GameObject.h"
#include "World.h"
#include <vector>

class GameManager  : public GameObject
{
public:
	GameManager()
	{

	};

	~GameManager();

	static GameManager* GetInstance()
	{
		return m_instance = (m_instance != nullptr) ? m_instance : new GameManager();
	}

	void Init() override;

	void Update() override;

	void CreateLevel();


private:
	static GameManager* m_instance;

	World& world = GameEngine::GetEngine()->GetWorld();
};