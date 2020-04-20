#pragma once

#include "contracts.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class Terrain;
class Building;
class Tilemap;

class Tile
{
public:
	Tile(sf::Vector2i pos, const Terrain* terrain);

	sf::Vector2i getPos() const;

	const Terrain* getTerrain() const;
	void setTerrain(const Terrain* terrain);

	void setBuilding(const Building* building);
	const Building* getBuilding() const;

	sf::Color calculateColor(const Tilemap& tilemap) const;

	std::string getInfoStr() const;
private:
	sf::Vector2i myPos;
	const Terrain* myTerrain = nullptr;
	const Building* myBuilding = nullptr;

	BEGIN_INVARIANT(Tile)
		INVARIANT(self.myTerrain != nullptr);
	END_INVARIANT()
};
