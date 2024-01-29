#include <SDL.h>
#include "GameEngine.h"
#include "SDLWrapper.h"
#include "Window.h"
#include "Renderer.h"
#include "Texture.h"
#include <iostream>
#include <stdlib.h>
#include "Components.h"
#include "Input.h"
#include "Manager.h"
#include "World.h"
#include "LogOutput.h"
#include "ManagerRegistry.h"

Renderer* GameEngine::m_renderer = nullptr;
GameEngine* GameEngine::m_engine = nullptr;
SDL_Event GameEngine::event;

GameEngine::GameEngine()
{
	m_isRunning = false;
	m_isActive = false;
	m_engine = this;
	m_world = std::make_unique<World>();

	frameStart = 0.f;
	currentTime = 0.f;
	deltaTime = 0.f;
	frameTime = 0.f;
	frameRate = 60.f;
}

GameEngine::~GameEngine()
{
	Cleanup();
}

void GameEngine::Init(const char* windowTitle, int windowWidth, int windowHeight, bool isFullScreen)
{
	std::cout << "\033[0m";

	frameStart = 0.f;
	currentTime = 0.f;
	frameTime = 0.f;
	deltaTime = 0.f;
	frameRate = 60.f;

	srand(time(NULL));

	int flag = 0;

	InitializeSDL();
	InitializeWindow(windowTitle, windowWidth, windowHeight, isFullScreen);
	InitializeRenderer();

	m_isActive = true;

	std::cout << "Engine Init" << std::endl;

	if (!m_world)
	{
		DebugLog(LogMessage::ERROR, "Failed to create World!");
		return;
	}

	m_world->Init();
}

void GameEngine::Run()
{
	m_isRunning = true;

	//Engine Loop
	while (m_isRunning)
	{
		frameStart = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - frameStart) / 1000.0f;

		frameTime += deltaTime;

		if (frameTime >= (1.0f / frameRate))
		{
			HandleEvents();
			Update();
			frameTime = 0.f;
		}

		m_world->Update(deltaTime);
		Render();
		m_window->updateSurface();
	}

	Shutdown();
}

void GameEngine::Update()
{
	m_world->Refresh();
	m_world->UpdateObjects();
}

void GameEngine::HandleEvents()
{
	//Poll SDL Events
	SDL_PollEvent(&GameEngine::event);

	//Check events
	switch (GameEngine::event.type)
	{
	case SDL_QUIT:
		m_isRunning = false;
		break;
	}

	//Check for Input
	Input::GetInstance()->Listen();
}

void GameEngine::Render()
{
	SDL_SetRenderDrawColor(GameEngine::GetRenderer(), 0, 0, 0, 0);
	SDL_RenderClear(GameEngine::GetRenderer());
	m_world->Draw();
	SDL_RenderPresent(GameEngine::GetRenderer());
}

void GameEngine::Shutdown()
{
	SDL_DestroyRenderer(GameEngine::GetRenderer());
	SDL_DestroyWindow(m_window->GetWindow());

	Cleanup();

	std::cout << "Engine Shutdown" << std::endl;

	SDL_Quit();
}

void GameEngine::InitializeSDL()
{
	m_sdl = new SDLWrapper(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
}

void GameEngine::InitializeWindow(const char* windowTitle, int windowWidth, int windowHeight, bool isFullScreen)
{
	int flags = 0;

	if (isFullScreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	m_window = new Window(windowTitle, windowWidth, windowHeight, flags);
}

void GameEngine::InitializeRenderer()
{
	m_renderer = new Renderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void GameEngine::Cleanup()
{
	delete m_renderer;
	m_renderer = nullptr;

	delete m_window;
	m_window = nullptr;

	delete m_sdl;
	m_sdl = nullptr;
}

bool GameEngine::IsActive()
{
	return m_isActive;
}

GameEngine* GameEngine::GetEngine()
{
	return m_engine;
}

World& GameEngine::GetWorld()
{
	return *m_world;
}

SDL_Renderer* GameEngine::GetRenderer()
{
	return m_renderer->GetRenderer();
}

