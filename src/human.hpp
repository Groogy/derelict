#pragma once

#include "settler.hpp"

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <memory>

class Tile;
class Earth;

class Human
{
public:
	Human(sf::Vector2i settlement);

	void update(Earth& earth);
	bool isDestroyed() const;

private:
	void handleGrowth(Earth& earth);

	std::vector<sf::Vector2i> mySettlements;
	std::vector<std::unique_ptr<Settler>> mySettlers;
	int nTimeToGrowth;
};
