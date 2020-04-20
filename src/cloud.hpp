#pragma once

#include "tile.hpp"

#include <SFML/System/Vector2.hpp>

#include <vector>

namespace sf
{
	class Image;
}

class Earth;
class Tilemap;

class Cloud
{
public:
	Cloud(sf::Vector2i pos, int volume, bool pollution, const Tilemap& tilemap);

	bool update(Earth& earth, sf::Image& colorCache);

	const std::vector<sf::Vector2i>& getParts() const;

	bool isPollution() const;

	void clearFromImage(sf::Image& colorCache) const;
	void applyToImage(sf::Image& colorCache) const;

private:
	void generateParts(int count, const Tilemap& tilemap, std::vector<sf::Vector2i>& potential);
	int calculateSpeed() const;

	void doDrift(const Earth& earth, sf::Image& colorCache);
	void driftParts(const Earth& earth, std::vector<sf::Vector2i>& toDrift, sf::Vector2i dir);

	void doRain(Earth& earth, sf::Image& colorCache);
	void handleGoodRain(sf::Vector2i part, Tilemap& tilemap);
	void handleBadRain(sf::Vector2i part, Tilemap& tilemap);

	std::vector<sf::Vector2i> myParts;
	sf::Vector2f myDriftVector;
	int myTicksToDrift;
	bool myIsPollution;
};

