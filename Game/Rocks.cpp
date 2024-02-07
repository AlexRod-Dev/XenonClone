#include "Rocks.h"
#include "Vector2D.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

Rocks::Rocks(bool isAlive)
{
	scrollingSpeed = 3.0f;
	isActive = isAlive;
}

Rocks::~Rocks()
{

}

void Rocks::Init()
{
	__super::Init();

	SetName("Rocks");

	whichRocks = rand() % 2;
	std::cout << whichRocks << std::endl;
	if (whichRocks == 0)
	{
		horizontalMargin = 1200;
		//verticalMargin =(rand() % (0 - 200 + 1));
		verticalMargin = -100;
		std::cout << horizontalMargin << std::endl;
		std::cout << verticalMargin << std::endl;
		AddComponent<TransformComponent>(horizontalMargin, verticalMargin);
		AddComponent<SpriteComponent>("../Assets/graphics/BlocksA.bmp", false, false);
	}
	else if (whichRocks == 1)
	{
		horizontalMargin = 1200;
		verticalMargin =-700;

		AddComponent<TransformComponent>(horizontalMargin, verticalMargin);
		AddComponent<SpriteComponent>("../Assets/graphics/BlocksB.bmp", false, false);
	}

	spriteComponent = &GetComponent<SpriteComponent>();
	transformComponent = &GetComponent<TransformComponent>();
}

void Rocks::Update()
{
	__super::Update();

	spriteComponent->ChangeSrcRect(1024, 2048);
	spriteComponent->ChangeDstRect(1024, 2048);

	transformComponent->velocity.x = -1 * scrollingSpeed;

	if (transformComponent->position.x < -1900)
	{
		Destroy();
	}
}
