#include "tile.hpp"
#include "terrain.hpp"
#include "building.hpp"

extern sf::Color TerrainPalette[];

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

void Tile::setBuilding(const Building* building)
{
	myBuilding = building;
}

const Building* Tile::getBuilding() const
{
	return myBuilding;
}

sf::Color Tile::calculateColor() const
{
	int nColorId = myTerrain->getColorId();
	return sf::Color(nColorId, myBuilding != nullptr, 0, 0);
}

std::string Tile::getInfoStr() const
{
	std::string result = "Terrain: " + myTerrain->getName() + "\n";
	result += "Building:" + (myBuilding ? myBuilding->getName() : "None") + "\n";
	return result;
}
