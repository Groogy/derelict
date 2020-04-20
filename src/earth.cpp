#include "earth.hpp"
#include "macros.hpp"
#include "building.hpp"

#include <iostream>

#include <cmath>

Earth::Earth()
: myTilemap("earth.png")
{
	generateSphere(10.0);
}

Earth::~Earth()
{
	
}

const Tilemap& Earth::getTilemap() const
{
	return myTilemap;
}

Tilemap& Earth::accessTilemap()
{
	return myTilemap;
}

const Homeostasis& Earth::getHomeostasis() const
{
	return myHomeostasis;
}

Homeostasis& Earth::accessHomeostasis()
{
	return myHomeostasis;
}

const Energy& Earth::getEnergy() const
{
	return myEnergy;
}

Energy& Earth::accessEnergy()
{
	return myEnergy;
}

bool Earth::canBuild(const Building& building) const
{
	if(myEnergy.getValue() < building.getEnergyCost())
		return false;

	return true;
}

bool Earth::canBuildOn(sf::Vector2i pos, const Building& building) const
{
	if(!canBuild(building))
		return false;

	auto tile = myTilemap.getTile(pos);
	if(tile.getBuilding())
		return false;

	if(!tile.getTerrain()->canBuild(building.getName()))
		return false;

	return true;
}

void Earth::spawnCloud(sf::Vector2i pos, int volume, bool pollution)
{
	myTilemap.createCloud(pos, volume, pollution);
}

void Earth::spawnHuman(sf::Vector2i pos, const Building* building)
{
	myTilemap.setTileTerrain(pos, "Settlement");
	myTilemap.setBuilding(pos, building);
	myHumans.emplace_back(std::make_unique<Human>(pos));
}

bool Earth::hasHumans() const
{
	return !myHumans.empty();
}

void Earth::buildOn(sf::Vector2i pos, const Building& building)
{
	myTilemap.setBuilding(pos, &building);
	myEnergy.add(-building.getEnergyCost());
}

void Earth::update()
{
	myTilemap.update(*this);
	myHomeostasis.update(*this);
	myEnergy.update(*this);

	updateHumans();
}

void Earth::updateHumans()
{
	std::vector<Human*> destroyedHumans;
	for(auto& human : myHumans)
	{
		if(human->isDestroyed())
		{
			destroyedHumans.push_back(human.get());
		}
		else
		{
			human->update(*this);
		}
	}
	for(auto destroyed : destroyedHumans)
	{
		for(auto& human : myHumans)
		{
			if(human.get() == destroyed)
			{
				myHumans.erase(macros::find(myHumans, human));
			}
		}
	}
}

void Earth::generateSphere(float radius)
{
	constexpr float Pi = 3.141592654f;
	constexpr int SectorCount = 192;
	constexpr int StackCount = 192;
	constexpr float SectorStep = 2 * Pi / SectorCount;
	constexpr float StackStep = Pi / StackCount;

	std::vector<Vertex> tmpVertices;
	
	for(int stack = 0; stack <= StackCount; stack++)
	{
		float stackAngle = Pi / 2 - stack * StackStep;
		float xy = radius * std::cos(stackAngle);
		float z = radius * std::sin(stackAngle);
		for(int sector = 0; sector <= SectorCount; sector++)
		{
			float sectorAngle = sector * SectorStep;
			float x = xy * std::cos(sectorAngle);
			float y = xy * std::sin(sectorAngle);
			tmpVertices.emplace_back(
				sf::Vector3f(x, y, z), 
				sf::Vector2f(static_cast<float>(sector + 1) / SectorCount, static_cast<float>(stack + 1) / StackCount)
			);
		}
	}

	int index = 0;
	for(int stack = 0; stack < StackCount; stack++)
	{
		int k1 = stack * (SectorCount + 1);
		int k2 = (stack + 1) * (SectorCount + 1);
		for(int sector = 0; sector < SectorCount; sector++, k1++, k2++)
		{
			auto v1 = tmpVertices[k1];
			auto v2 = tmpVertices[k2];
			auto v3 = tmpVertices[k1 + 1];
			auto v4 = tmpVertices[k2 + 1];

			if(stack == 0)
			{
				myVertices.push_back(v2);
				myVertices.push_back(v1);
				myVertices.push_back(v4);

				myIndices.push_back(index++);
				myIndices.push_back(index++);
				myIndices.push_back(index++);
			}
			else if(stack == StackCount - 1)
			{
				myVertices.push_back(v1);
				myVertices.push_back(v2);
				myVertices.push_back(v3);

				myIndices.push_back(index++);
				myIndices.push_back(index++);
				myIndices.push_back(index++);
			}
			else
			{
				myVertices.push_back(v1);
				myVertices.push_back(v2);
				myVertices.push_back(v3);
				myVertices.push_back(v4);

				myIndices.push_back(index);
				myIndices.push_back(index+1);
				myIndices.push_back(index+2);

				myIndices.push_back(index+2);
				myIndices.push_back(index+1);
				myIndices.push_back(index+3);
				
				index += 4;
			}
			
		}
	}
}