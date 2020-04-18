#pragma once

#include "renderer.hpp"
#include "earth.hpp"
#include "player_input.hpp"

namespace sf
{
	class RenderWindow;
}

class GameState
{
public:
	GameState(sf::RenderWindow& window);
	~GameState();

	bool isRunning() const;

	void update();

private:
	void handleEvents();
	void handleTick();
	void handleRender();
	void handleUI();

	sf::RenderWindow& myWindow;
	Renderer myRenderer;
	PlayerInput myInput;
	Earth myEarth;
};
