#include "terrain.hpp"

Terrain::Terrain(const std::string& name, sf::Color color)
: myName(name)
, myColor(color)
{

}

const std::string& Terrain::getName() const
{
	return myName;
}

sf::Color Terrain::getColor() const
{
	return myColor;
}