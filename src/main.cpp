#include "contracts.hpp"

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>

int main()
{
	sf::Window window(sf::VideoMode(800, 600), "My Window");
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
	}
	return 0;
}
