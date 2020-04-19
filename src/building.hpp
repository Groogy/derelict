#pragma once 

#include <functional>
#include <string>

class Earth;

class Building
{
public:
	using UpdateEffect = std::function<void(Earth&)>;

	Building(const std::string& name, const std::string& button, float energyCost, float homeostasisChange, float energyChange, UpdateEffect update = nullptr);

	const std::string& getName() const;
	const std::string& getButton() const;
	float getEnergyCost() const;
	float getHomeostasisChange() const;
	float getEnergyChange() const;

	std::string getStatsStr() const;

	void update(Earth& earth) const;
	
private:
	std::string myName;
	std::string myButton;
	float myEnergyCost;
	float myHomeostasisChange;
	float myEnergyChange;

	UpdateEffect myUpdateEffect;
};
