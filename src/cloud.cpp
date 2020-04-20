#include "cloud.hpp"
#include "macros.hpp"
#include "earth.hpp"
#include "tilemap.hpp"

#include <iostream>

Cloud::Cloud(sf::Vector2i pos, int volume, bool pollution, const Tilemap& tilemap)
: myIsPollution(pollution)
{
	myParts.push_back(pos);
	std::vector<sf::Vector2i> potentials;
	generateParts(volume - 1, tilemap, potentials);

	myDriftVector.x = static_cast<float>(rand() % 100 - 50) / 10.f;
	myDriftVector.y = static_cast<float>(rand() % 100 - 50) / 10.f;
	myTicksToDrift = calculateSpeed();
}

bool Cloud::update(Earth& earth, sf::Image& image)
{
	myTicksToDrift -= 1;
	if(myTicksToDrift <= 0)
	{
		doDrift(earth, image);
		myTicksToDrift = calculateSpeed();
		return true;
	}
	doRain(earth, image);
	return false;
}

const std::vector<sf::Vector2i>& Cloud::getParts() const
{
	return myParts;
}

bool Cloud::isPollution() const
{
	return myIsPollution;
}

void Cloud::generateParts(int count, const Tilemap& tilemap, std::vector<sf::Vector2i>& potential)
{
	const sf::Vector2i Dir[] = {
		sf::Vector2i(-1, 0), sf::Vector2i(1, 0),
		sf::Vector2i(0, 1), sf::Vector2i(0, -1),
		sf::Vector2i(-1, -1), sf::Vector2i(1, 1),
		sf::Vector2i(-1, 1), sf::Vector2i(1, -1),
	};
	for(auto part : myParts)
	{
		for(auto dir : Dir)
		{
			auto neighbour = part + dir;
			if(macros::contains(myParts, neighbour) || macros::contains(potential, neighbour))
				continue;

			if(!tilemap.isInside(neighbour))
				continue;

			if(!tilemap.canHaveCloud(neighbour))
				continue;

			potential.push_back(neighbour);
		}
	}
	if(potential.empty())
		return;

	int random = rand() % potential.size();
	myParts.push_back(potential[random]);
	potential.erase(potential.begin() + random);
	if(count > 1)
		generateParts(count - 1, tilemap, potential);
}

int Cloud::calculateSpeed() const
{
	return myDriftVector.x * myDriftVector.x + myDriftVector.y * myDriftVector.y;
}

void Cloud::doDrift(const Earth& earth, sf::Image& colorCache)
{
	clearFromImage(colorCache);
	std::vector<sf::Vector2i> toDrift = myParts;
	myParts.clear();

	sf::Vector2i dir;
	if(myDriftVector.x > 0 && rand() % 5 < std::abs(myDriftVector.x))
		dir.x = 1 ;
	else if(myDriftVector.x < 0 && rand() % 5 < std::abs(myDriftVector.x))
		dir.x = -1;
	if(myDriftVector.y > 0 && rand() % 5 < std::abs(myDriftVector.y))
		dir.y = 1;
	else if(myDriftVector.y < 0 && rand() % 5 < std::abs(myDriftVector.y))
		dir.y = -1;

	driftParts(earth, toDrift, dir);
	applyToImage(colorCache);
}

void Cloud::driftParts(const Earth& earth, std::vector<sf::Vector2i>& toDrift, sf::Vector2i dir)
{
	auto& tilemap = earth.getTilemap();
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> canDrift;
	for(auto part : toDrift)
	{
		sf::Vector2i newPos = part + dir;
		if(!tilemap.isInside(newPos))
			continue;
		if(macros::contains(myParts, newPos))
			continue;
		if(!tilemap.canHaveCloud(newPos))
			continue;
		
		canDrift.push_back(std::make_pair(part, newPos));
	}

	if(canDrift.empty())
	{
		myParts.insert(myParts.end(), toDrift.begin(), toDrift.end());
	}
	else
	{
		for(auto pair : canDrift)
		{
			toDrift.erase(macros::find(toDrift, pair.first));
			myParts.push_back(pair.second);
		}
		driftParts(earth, toDrift, dir);
	}
}

void Cloud::clearFromImage(sf::Image& colorCache) const
{
	for(auto part : myParts)
	{
		auto color = colorCache.getPixel(part.x, part.y);
		color.b = 0;
		colorCache.setPixel(part.x, part.y, color);
	}
}

void Cloud::applyToImage(sf::Image& colorCache) const
{
	for(auto part : myParts)
	{
		auto color = colorCache.getPixel(part.x, part.y);
		color.b = myIsPollution ? 2 : 1;
		colorCache.setPixel(part.x, part.y, color);
	}
}

void Cloud::doRain(Earth& earth, sf::Image& colorCache)
{
	const sf::Vector2i Dir[] = {
		sf::Vector2i(-1, 0),
		sf::Vector2i(1, 0),
		sf::Vector2i(0, 1),
		sf::Vector2i(0, -1),
	};
	auto& tilemap = earth.accessTilemap();
	std::vector<sf::Vector2i> toRemove;
	for(auto part : myParts)
	{
		int chance = 500;
		for(auto dir : Dir)
		{
			auto neighbour = part + dir;
			if(!macros::contains(myParts, neighbour))
				chance /= 2;
		}
		int random = rand() % chance;

		if(random == 0)
		{
			if(myIsPollution)
				handleBadRain(part, tilemap);
			else
				handleGoodRain(part, tilemap);
			
			toRemove.push_back(part);
		}
	}
	for(auto part : toRemove)
	{
		myParts.erase(macros::find(myParts, part));
		auto color = colorCache.getPixel(part.x, part.y);
		color.b = 0;
		colorCache.setPixel(part.x, part.y, color);
	}
}

void Cloud::handleGoodRain(sf::Vector2i part, Tilemap& tilemap)
{
	auto tile = tilemap.findFirstConvertable(part, "Water", 256);
	if(tile.getTerrain()->canSwitchTo("Water"))
	{
		tilemap.setTileTerrain(part, "Water");
	}
	tile = tilemap.findFirstConvertable(part, "Restored", 256);
	if(tile.getTerrain()->canSwitchTo("Restored"))
	{
		tilemap.setTileTerrain(part, "Restored");
	}
}

void Cloud::handleBadRain(sf::Vector2i part, Tilemap& tilemap)
{
	auto tile = tilemap.findFirstConvertable(part, "Wasteland", 256);
	if(tile.getTerrain()->canSwitchTo("Wasteland"))
	{
		tilemap.setTileTerrain(part, "Wasteland");
	}

	tile = tilemap.findFirstConvertable(part, "Ocean Bed", 256);
	if(tile.getTerrain()->canSwitchTo("Ocean Bed"))
	{
		tilemap.setTileTerrain(part, "Ocean Bed");
	}
}
