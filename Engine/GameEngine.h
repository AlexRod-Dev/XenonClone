#pragma once
#include <memory>
#include <string>

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	void Init(const char* windowTitle, int windowWidth, int windowHeight, bool isFullScreen);

	int main() {
		return 0;
	}
	void Run();

	void Update();

	void HandleEvents();

	void Render();

	void Shutdown();

	bool IsActive();

	static class GameEngine* GetEngine();

	World& GetWorld();

	inline float GetDeltaTime() { return deltaTime; }

	static SDL_Renderer* GetRenderer();
	static SDL_Event event;

private:
	void InitializeSDL();
	void InitializeWindow(const char* windowTitle, int windowWidth, int windowHeight, bool isFullScreen);
	void InitializeRenderer();
	void Cleanup();

	SDLWrapper* m_sdl = nullptr;
	Window* m_window = nullptr;
	static Renderer* m_renderer;

	std::unique_ptr<World> m_world;

	bool m_isRunning;
	bool m_isActive;

	static GameEngine* m_engine;

	float frameStart;
	int currentTime;
	float frameTime;
	float deltaTime;
	float frameRate;

};

