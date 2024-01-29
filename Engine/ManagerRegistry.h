#pragma once

#include <vector>
#include <memory>

class Manager;

class ManagerRegistry
{
public:

	static void Init();
	static void Update(float deltaTime);

private:
	static std::vector<std::unique_ptr<Manager>> m_managers;

	static void RegisterManager(std::unique_ptr<Manager> manager);
};