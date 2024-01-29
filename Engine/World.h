#pragma once

#include "../include/Box2D/box2d/box2d.h"
#include "EntityComponentSystem.h"
#include <memory>
#include <queue>
#include <functional>

class World : public b2ContactListener
{
public:

	World();
	~World();

	/*static World* GetInstance()
	{
		return m_instance = m_instance != nullptr ? m_instance : new World();
	}*/

	void Init();
	void Update(float deltaTime);
	void Draw();
	void Refresh();
	void UpdateObjects();

	virtual void BeginContact(b2Contact* contact) override;
	virtual void EndContact(b2Contact* contact) override;

	void QueueAction(const std::function<void()>& action);

	// Add an entity to a group
	void AddToGroup(Entity* entity, Group group)
	{
		m_groupedEntities[group].emplace_back(entity);
	}

	// Returns a list of entities inside a group
	std::vector<Entity*>& GetGroup(Group group)
	{
		return m_groupedEntities[group];
	}

	// Create an entity of type T and add it to the entities vector
	template <typename T, typename... TArgs>
	T* CreateEntity(TArgs&& ...mArgs)
	{
		T* obj(new T(std::forward<TArgs>(mArgs)...));
		std::unique_ptr<Entity> uPtr{ obj };
		m_entities.emplace_back(std::move(uPtr));
		obj->Init();

		return obj;
	}

	void SetGravity(b2Vec2 newGravity);

	inline b2Vec2 GetGravity() { return gravity; }

	inline b2World* GetB2World() { return m_world.get(); }


private:
	static std::unique_ptr<World> m_instance;

	std::unique_ptr<b2World> m_world;

	b2Vec2 gravity;

	std::vector<std::function<void()>> m_actions;

	std::vector<std::unique_ptr<Entity>> m_entities;

	std::array<std::vector<Entity*>, maxGroups> m_groupedEntities;
};