#include "tile.hpp"
#include "terrain.hpp"

Tile::Tile(sf::Vector2i pos, const Terrain* terrain)
: myPos(pos)
, myTerrain(terrain)
{
}

sf::Vector2i Tile::getPos() const
{
	return myPos;	
}

const Terrain* Tile::getTerrain() const
{
	return myTerrain;
}

std::string Tile::getInfoStr() const
{
	std::string result = "Terrain: ";
	result += myTerrain->getName();
	return result;
}