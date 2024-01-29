#pragma once

class Manager
{
public:
	Manager() = default;
	~Manager();

	virtual void Start();
	virtual void Update(float deltaTime);
};