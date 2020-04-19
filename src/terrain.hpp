#pragma once

#include <SFML/Graphics/Color.hpp>

#include <string>

class Terrain
{
public:
	Terrain(const std::string& name, int colorId);

	const std::string& getName() const;
	sf::Color getColor() const;
	int getColorId() const;

private:
	std::string myName;
	int myColorId;
};
