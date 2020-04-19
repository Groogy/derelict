#include "energy.hpp"
#include "earth.hpp"
#include "building.hpp"

void Energy::update(const Earth& earth)
{
	myValue += calculateChange(earth);
}

float Energy::calculateChange(const Earth& earth) const
{
	constexpr float BaseGain = 0.05;
	float change = BaseGain;
	for(auto tile : earth.getTilemap().getTiles())
	{
		auto building = tile.getBuilding();
		if(building)
		{
			change += building->getEnergyChange();
		}
	}
	return change;
}

float Energy::getValue() const
{
	return myValue;
}

void Energy::add(float value)
{
	myValue += value;
}