#include "gamestate.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <cstdlib>

int main()
{
	std::srand(std::time(nullptr));
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;

	sf::RenderWindow window(sf::VideoMode(1280, 800), "Ludum Dare 46", sf::Style::Default, settings);
	GameState game(window);
	while(game.isRunning())
	{
		game.update();
		window.display();
	}
	return 0;
}
