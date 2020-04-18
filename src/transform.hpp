#pragma once

#include <SFML/System/Vector2.hpp>

struct Transform
{
	static Transform Identity;

	float offset;
	sf::Vector2f rotation;
};
