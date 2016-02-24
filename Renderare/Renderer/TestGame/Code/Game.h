#pragma once

class Window;
class D3DRenderer;
class GameTimer;

class Game
{
public:
	Game();
	~Game();

	Window* get_window() const;
	D3DRenderer* get_renderer() const;
	int Run();

private:
	void Update();
	void Render();
	void CleanUp();

	Window* window_;
	GameTimer* game_timer_;
	D3DRenderer* renderer_;
};