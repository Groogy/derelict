#pragma once

#include "renderer.hpp"
#include "earth.hpp"
#include "player_input.hpp"
#include "camera2d.hpp"
#include "topbar.h"

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

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
	void handleEvents(sf::Time delta);
	void handleTick();
	void handleRender();
	void handleUI();

	sf::RenderWindow& myWindow;
	sf::Clock myClock;
	sf::Time myTimeSinceLastTick;
	Renderer myRenderer;
	PlayerInput myInput;
	Earth myEarth;

	Camera2D myCamera;

	sf::Shader myEarthShader;
	sf::Shader myTilemapShader;

	Topbar myTopbar;
};
