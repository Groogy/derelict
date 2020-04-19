#pragma once

#include "contracts.hpp"

#include <SFML/System/Vector2.hpp>

class Terrain;

class Tile
{
public:
	Tile(sf::Vector2i pos, const Terrain* terrain);

	sf::Vector2i getPos() const;
	const Terrain* getTerrain() const;

	std::string getInfoStr() const;
private:
	sf::Vector2i myPos;
	const Terrain* myTerrain;

	BEGIN_INVARIANT(Tile)
		INVARIANT(self.myTerrain != nullptr);
	END_INVARIANT()
};
