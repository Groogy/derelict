#include "terrain.hpp"
#include "macros.hpp"

extern std::vector<sf::Color>  TerrainPalette;

Terrain::Terrain(const std::string& name, int colorId, bool canHaveCloud, bool impassable, float homeostasisChange, const std::vector<std::string>& allowedBuildings, const std::vector<std::string>& allowedConversions)
: myName(name)
, myColorId(colorId)
, myCanHaveCloud(canHaveCloud)
, myImpassable(impassable)
, myHomeostasisChange(homeostasisChange)
, myAllowedBuildings(allowedBuildings)
, myAllowedConversion(allowedConversions)
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

bool Terrain::canHaveCloud() const
{
	return myCanHaveCloud;
}

bool Terrain::isImpassable() const
{
	return myImpassable;
}

float Terrain::getHomeostasisChange() const
{
	return myHomeostasisChange;
}

bool Terrain::canBuild(const std::string& building) const
{
	return macros::contains(myAllowedBuildings, building);
}

bool Terrain::canSwitchTo(const std::string& type) const
{
	return macros::contains(myAllowedConversion, type);
}