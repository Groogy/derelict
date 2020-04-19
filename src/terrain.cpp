#include "terrain.hpp"

extern sf::Color TerrainPalette[];

Terrain::Terrain(const std::string& name, int colorId)
: myName(name)
, myColorId(colorId)
{

}

const std::string& Terrain::getName() const
{
	return myName;
}

sf::Color Terrain::getColor() const
{
	return TerrainPalette[myColorId];
}

int Terrain::getColorId() const
{
	return myColorId;
}