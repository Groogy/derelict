#include "homeostasis.hpp"
#include "earth.hpp"
#include "building.hpp"

void Homeostasis::update(const Earth& earth)
{
	myValue += calculateChange(earth);
}

float Homeostasis::calculateChange(const Earth& earth) const
{
	constexpr float BaseDecay = -0.01;
	float change = BaseDecay;
	for(auto tile : earth.getTilemap().getTiles())
	{
		auto building = tile.getBuilding();
		if(building)
		{
			change += building->getHomeostasisChange();
		}
		change += tile.getTerrain()->getHomeostasisChange();
	}
	return change;
}

float Homeostasis::getValue() const
{
	return myValue;
}
