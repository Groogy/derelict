#include "player_input.hpp"

#include <SFML/Window/Event.hpp>

void PlayerInput::processEvent(const sf::Event& event)
{
	switch(event.type)
	{
	case sf::Event::KeyPressed:
		setActionFromKey(event.key.code, true);
		break;
	case sf::Event::KeyReleased:
		setActionFromKey(event.key.code, false);
		break;
	default:
		break;
	}
}

float PlayerInput::getCameraOffsetChange() const
{
	float change = 0;
	if(myActions[In])
		change += -1;
	if(myActions[Out])
		change += 1;
	return change;
}

sf::Vector2f PlayerInput::getCameraRotationChange() const
{
	sf::Vector2f change;
	if(myActions[Left])
		change.x += 1;
	if(myActions[Right])
		change.x -= 1;
	if(myActions[Up])
		change.y += 1;
	if(myActions[Down])
		change.y -= 1;
	return change;
}

void PlayerInput::setActionFromKey(sf::Keyboard::Key key, bool state)
{
	switch(key)
	{
	case sf::Keyboard::A:
	case sf::Keyboard::Left:
		myActions[Left] = state;
		break;
	case sf::Keyboard::D:
	case sf::Keyboard::Right:
		myActions[Right] = state;
		break;
	case sf::Keyboard::W:
	case sf::Keyboard::Up:
		myActions[Up] = state;
		break;
	case sf::Keyboard::S:
	case sf::Keyboard::Down:
		myActions[Down] = state;
		break;
	case sf::Keyboard::Q:
	case sf::Keyboard::PageUp:
		myActions[In] = state;
		break;
	case sf::Keyboard::E:
	case sf::Keyboard::PageDown:
		myActions[Out] = state;
		break;
	default:
		break;
	}
}
