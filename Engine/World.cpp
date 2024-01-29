#include "World.h"
#include "EntityComponentSystem.h"
#include "LogOutput.h"

std::unique_ptr<World> World::m_instance = nullptr;

World::World() : m_world(nullptr)
{

}

World::~World()
{
	/*delete m_instance;*/
}

void World::Init()
{
	SetGravity(b2Vec2(0.0f, 0.0f));
	m_world = std::make_unique<b2World>(gravity);

	m_world->SetContactListener(this);
	m_world->SetContinuousPhysics(true);
	m_world->SetAllowSleeping(false);

	for (const auto& entity : m_entities)
	{
		entity->Init();
	}

	std::cout << "World Init" << std::endl;
}

void World::Update(float deltaTime)
{
	m_world->DebugDraw();
	m_world->Step(deltaTime, 12, 8);

	for (const auto& action : m_actions)
	{
		action();
	}
	m_actions.clear();
}

void World::Draw()
{
	for (const auto& entity : m_entities)
	{
		entity->Draw();
	}
}

void World::Refresh()
{
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
		[](const std::unique_ptr<Entity>& mEntity)
		{
			return !mEntity->IsActive();
		}),
		m_entities.end());
}

void World::UpdateObjects()
{
	for (int i = 0; i < m_entities.size(); ++i)
	{
		m_entities[i]->Update();
	}
}

void World::QueueAction(const std::function<void()>& action)
{
	m_actions.push_back(action);
}

void World::SetGravity(b2Vec2 newGravity)
{
	gravity = newGravity;
}

void World::BeginContact(b2Contact* contact)
{
	Entity* bodyA = ((Entity*)contact->GetFixtureA()->GetUserData().pointer);
	Entity* bodyB = ((Entity*)contact->GetFixtureB()->GetUserData().pointer);

	if (bodyA != nullptr && bodyB != nullptr)
	{
		bodyA->BeginOverlap(bodyB);
		bodyB->BeginOverlap(bodyA);
	}
}

void World::EndContact(b2Contact* contact)
{
	Entity* bodyA = ((Entity*)contact->GetFixtureA()->GetUserData().pointer);
	Entity* bodyB = ((Entity*)contact->GetFixtureB()->GetUserData().pointer);

	if (bodyA != nullptr && bodyB != nullptr)
	{
		((Entity*)bodyA)->EndOverlap(((Entity*)bodyB));
		((Entity*)bodyB)->EndOverlap(((Entity*)bodyA));
	}
}
