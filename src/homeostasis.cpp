#include "homeostasis.hpp"
#include "earth.hpp"

void Homeostasis::update(const Earth& earth)
{
	myValue += calculateChange(earth);
}

float Homeostasis::calculateChange(const Earth&) const
{
	constexpr float BaseDecay = -0.01;
	return BaseDecay;
}

float Homeostasis::getValue() const
{
	return myValue;
}
