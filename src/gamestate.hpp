#pragma once

#include "renderer.hpp"
#include "earth.hpp"
#include "player_input.hpp"

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
	void handleEvents();
	void handleTick();
	void handleRender();
	void handleUI();

	sf::RenderWindow& myWindow;
	sf::Clock myClock;
	Renderer myRenderer;
	PlayerInput myInput;
	Earth myEarth;

	sf::Shader myEarthShader;
	sf::Texture myEarthTexture;
};
