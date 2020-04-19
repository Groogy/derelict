#include "energy.hpp"
#include "earth.hpp"

void Energy::update(const Earth& earth)
{
	myValue += calculateChange(earth);
}

float Energy::calculateChange(const Earth&) const
{
	constexpr float BaseGain = 0.05;
	return BaseGain;
}

float Energy::getValue() const
{
	return myValue;
}
