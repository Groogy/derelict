#pragma once

#include <SFML/Graphics/Color.hpp>

#include <string>

class Terrain
{
public:
	Terrain(const std::string& name, sf::Color color);

	const std::string& getName() const;
	sf::Color getColor() const;

private:
	std::string myName;
	sf::Color myColor;
};
