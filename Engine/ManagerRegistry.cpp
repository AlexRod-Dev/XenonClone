#include "ManagerRegistry.h"
#include "Manager.h"
#include <iostream>

std::vector<std::unique_ptr<Manager>> ManagerRegistry::m_managers;

void ManagerRegistry::Init()
{
	m_managers.push_back(std::make_unique<Manager>());
}

void ManagerRegistry::Update(float deltaTime)
{
	std::cout << m_managers.size() << std::endl;
	for (const auto& manager : m_managers)
	{
		manager->Update(deltaTime);
	}
}

void ManagerRegistry::RegisterManager(std::unique_ptr<Manager> manager)
{
	m_managers.push_back(std::move(manager));
}