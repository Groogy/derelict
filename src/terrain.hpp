#pragma once

#include <SFML/Graphics/Color.hpp>

#include <string>
#include <vector>

class Terrain
{
public:
	Terrain(const std::string& name, int colorId, bool canHaveCloud, bool impassable, float homeostasisChange, const std::vector<std::string>& allowedBuildings, const std::vector<std::string>& allowedConversion);

	const std::string& getName() const;
	sf::Color getColor() const;
	int getColorId() const;
	bool canHaveCloud() const;
	bool isImpassable() const;
	float getHomeostasisChange() const;

	bool canBuild(const std::string& building) const;
	bool canSwitchTo(const std::string& type) const;

private:
	std::string myName;
	int myColorId;
	bool myCanHaveCloud;
	bool myImpassable;
	float myHomeostasisChange;

	std::vector<std::string> myAllowedBuildings;
	std::vector<std::string> myAllowedConversion;
};
