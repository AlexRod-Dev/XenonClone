#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
	__super::Init();
}

void GameObject::Update()
{
	__super::Update();
}

void GameObject::Destroyed(Entity* entity)
{
	entity->Destroy();
}
