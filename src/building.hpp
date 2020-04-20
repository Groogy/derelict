#pragma once 

#include <functional>
#include <string>

class Earth;
class Tile;

class Building
{
public:
	using UpdateEffect = std::function<void(Earth&, const Tile& tile)>;

	Building(const std::string& name, const std::string& button, float energyCost, float homeostasisChange, float energyChange, bool hidden, UpdateEffect update = nullptr);

	const std::string& getName() const;
	const std::string& getButton() const;
	float getEnergyCost() const;
	float getHomeostasisChange() const;
	float getEnergyChange() const;
	bool isHidden() const;

	std::string getStatsStr() const;

	void update(Earth& earth, const Tile& tile) const;
	
private:
	std::string myName;
	std::string myButton;
	float myEnergyCost;
	float myHomeostasisChange;
	float myEnergyChange;
	bool myHidden;

	UpdateEffect myUpdateEffect;
};
