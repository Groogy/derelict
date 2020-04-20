#include "human.hpp"
#include "tile.hpp"
#include "terrain.hpp"
#include "earth.hpp"
#include "tilemap.hpp"
#include "settler.hpp"

constexpr int TicksPerSettlement = 40;

Human::Human(sf::Vector2i settlement)
{
	mySettlements.push_back(settlement);
	nTimeToGrowth = TicksPerSettlement;
}

void Human::update(Earth& earth)
{
	auto& tilemap = earth.accessTilemap();
	std::vector<sf::Vector2i> destroyedSettlements;
	for(auto settlement : mySettlements)
	{
		const auto& tile = tilemap.getTile(settlement);
		if(tile.getTerrain()->getName() != "Settlement")
		{
			destroyedSettlements.push_back(settlement);
		}
		else
		{
			nTimeToGrowth -= rand() % 2 == 0 ? 1 : 0;
		}
	}

	for(auto destroyed : destroyedSettlements)
	{
		mySettlements.erase(macros::find(mySettlements, destroyed));
	}

	if(nTimeToGrowth <= 0)
	{
		handleGrowth(earth);
		nTimeToGrowth = TicksPerSettlement * mySettlements.size();
	}

	std::vector<Settler*> doneSettlers;
	for(auto& settler : mySettlers)
	{
		if(settler->update(earth))
		{
			doneSettlers.push_back(settler.get());
		}
	}


	auto building = tilemap.getTile(mySettlements.front()).getBuilding();
	for(auto done : doneSettlers)
	{
		
		auto pos = done->getPosition();
		tilemap.setTileTerrain(pos, "Settlement");
		tilemap.setBuilding(pos, building);
		mySettlements.push_back(done->getPosition());
		for(auto& settler : mySettlers)
		{
			if(settler.get() == done)
			{
				mySettlers.erase(macros::find(mySettlers, settler));
			}
		}
	}
}

bool Human::isDestroyed() const
{
	return mySettlements.empty();
}

void Human::handleGrowth(Earth& earth)
{
	auto& tilemap = earth.getTilemap();

	int random = rand() % mySettlements.size();
	sf::Vector2i settlement = mySettlements[random];

	auto path = tilemap.findFirstConvertableWalkable(settlement, "Settlement");
	if(path.isFinished())
		return;

	auto settler = std::make_unique<Settler>(*this, settlement, path);
	mySettlers.push_back(std::move(settler));
}