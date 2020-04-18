#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <array>

namespace sf
{
	class Event;
}

class PlayerInput
{
public:
	void processEvent(const sf::Event& event);

	sf::Vector2f getCameraMovement() const;
	float getCameraZoomChange() const;

private:
	enum Action
	{
		Left,
		Right,
		Up,
		Down,
		In,
		Out,
		ActionCount
	};

	void setActionFromKey(sf::Keyboard::Key key, bool state);

	std::array<bool, ActionCount> myActions = { false };
};
