#include "GameEngine.h"
#include "GameManager.h"
#include "Vector2D.h"
#include <iostream>


int main(int argc, char ** argv)
{

	GameEngine engine;

	engine.Init("Xenon Clone", 1024, 800, false);

	engine.GetWorld().CreateEntity<GameManager>();

	engine.Run();

	return 0;
}