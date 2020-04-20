#include "intro_state.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio/Music.hpp>

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

	sf::RenderWindow window(sf::VideoMode(1280, 800), "Derelict: Gaia Protocol", sf::Style::Default, settings);

	sf::Music music;
	music.openFromFile("resources/music.wav");
	music.setVolume(5.0);
	music.setLoop(true);
	music.play();

	State* currentState = new IntroState(window);
	while(currentState)
	{
		auto newState = currentState->update();
		if(newState != currentState)
		{
			delete currentState;
			currentState = newState;
		}
		window.display();
	}
	return 0;
}
