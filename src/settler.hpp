#pragma once

#include "path.hpp"

#include <SFML/System/Vector2.hpp>

class Human;
class Earth;

namespace sf
{
	class Image;
}

class Settler
{
public:
	Settler(Human& faction, sf::Vector2i pos, const Path& path);

	sf::Vector2i getPosition() const;

	bool update(Earth& earth);

	void clearFromImage(sf::Image& colorCache) const;
	void applyToImage(sf::Image& colorCache) const;

private:
	Human& myFaction;
	sf::Vector2i myPosition;
	Path myPath;
};
