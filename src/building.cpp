#include "building.hpp"
#include "macros.hpp"

Building::Building(const std::string& name, const std::string& button, float energyCost, float homeostasisChange, float energyChange, UpdateEffect update)
: myName(name)
, myButton(button)
, myEnergyCost(energyCost)
, myHomeostasisChange(homeostasisChange)
, myEnergyChange(energyChange)
, myUpdateEffect(update)
{

}

const std::string& Building::getName() const
{
	return myName;
}

const std::string& Building::getButton() const
{
	return myButton;
}

float Building::getEnergyCost() const
{
	return myEnergyCost;
}

float Building::getHomeostasisChange() const
{
	return myHomeostasisChange;
}

float Building::getEnergyChange() const
{
	return myEnergyChange;
}

std::string Building::getStatsStr() const
{
	std::string result = myName + "\n";
	result += macros::displayFloat(myEnergyCost) + "\n";
	result += macros::displayFloatWithSign(myHomeostasisChange) + "/s\n";
	result += macros::displayFloatWithSign(myEnergyChange) + "/s\n";
	return result;
}

void Building::update(Earth& earth) const
{
	if(myUpdateEffect)
	{
		myUpdateEffect(earth);
	}
}