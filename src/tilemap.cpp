#include "tilemap.hpp"
#include "camera2d.hpp"
#include "building.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <iostream>

#include <deque>

constexpr int NumPaletteColors = 11;
std::vector<sf::Color> TerrainPalette = {
	sf::Color(0, 0, 0, 0), // Selected
	sf::Color(255, 69, 0), // Wasteland
	sf::Color(105, 105, 105), // Ruins
	sf::Color(0, 114, 152), // Glass
	sf::Color(50, 50, 50), // Mountains
	sf::Color(188, 143, 143), // Ocean Bed
	sf::Color(0, 26, 255), // Water
	sf::Color(70, 33, 4), // Barren
	sf::Color(0, 110, 9), // Restored
	sf::Color(255, 255, 0), // Settlement
	sf::Color(0, 255, 0), // Biosphere
};

Tilemap::Tilemap(const std::string& sourceFile)
{
	ENSURES_P(mySize.x > 0 && mySize.y > 0);

	sf::Image source;
	source.loadFromFile(sourceFile);
	mySize = source.getSize();
	myColorCache.create(mySize.x, mySize.y);
	myTexture.create(mySize.x, mySize.y);
	myTexture.setRepeated(true);
	setupTerrains();
	setupTiles(source);
	updateTexture();
}

sf::Vector2i Tilemap::getSize() const
{
	return static_cast<sf::Vector2i>(mySize);
}

void Tilemap::update(Earth& earth)
{
	for(auto& tile : myTiles)
	{
		if(tile.getBuilding())
		{
			auto building = tile.getBuilding();
			if(!tile.getTerrain()->canBuild(building->getName()))
			{
				setBuilding(tile.getPos(), nullptr);
			}
			else
			{
				building->update(earth, tile);
			}
		}
	}

	std::vector<const Cloud*> deadClouds;
	for(auto& cloud : myClouds)
	{
		if(cloud->update(earth, myColorCache))
		{
			myIsDirty = true;
		}

		if(cloud->getParts().empty())
			deadClouds.push_back(cloud.get());
	}
	for(auto dead : deadClouds)
	{
		for(auto it = myClouds.begin(); it != myClouds.end(); it++)
		{
			if(it->get() == dead)
			{
				myClouds.erase(it);
				break;
			}
		}
	}
}

const sf::Texture& Tilemap::getTexture() const
{
	return myTexture;
}

sf::Image& Tilemap::accessImage()
{
	myIsDirty = true;
	return myColorCache;
}

void Tilemap::prepareShader(sf::Shader& shader, const Camera2D& camera, sf::Time timeSinceStart) const
{
	shader.setUniform("terrainSampler", myTexture);
	shader.setUniform("cameraPosition", camera.getPosition());
	shader.setUniform("cameraZoom", camera.getZoom());
	shader.setUniform("time", timeSinceStart.asSeconds() / 10);
	shader.setUniform("showClouds", myShowClouds);

	sf::Glsl::Vec4 tmp[NumPaletteColors];
	for(int index = 0; index < NumPaletteColors; index++)
	{
		sf::Color color = TerrainPalette[index];
		tmp[index] = sf::Glsl::Vec4(
			static_cast<float>(color.r) / 256, 
			static_cast<float>(color.g) / 256, 
			static_cast<float>(color.b) / 256, 
			static_cast<float>(color.a) / 256
		);
	}
	shader.setUniformArray("colorPalette", tmp, NumPaletteColors);
}

const Terrain* Tilemap::findTerrainFromColor(sf::Color color) const
{
	for(auto& terrain : myTerrains)
	{
		if(terrain->getColor() == color)
			return terrain.get();
	}

	return nullptr;
}

const Tile& Tilemap::findTileFromClick(const sf::Vector2i click, const Camera2D& camera) const
{
	auto cameraZoom = camera.getZoom();
	sf::Vector2f tileSize(2 / cameraZoom, 2.3 / cameraZoom);
	sf::Vector2f tilemapSize(mySize.x * tileSize.x, mySize.y * tileSize.y);
	auto cameraPos = camera.getPosition();
	cameraPos.x *= tilemapSize.x;
	cameraPos.y *= tilemapSize.y;
	sf::Vector2f tilemapClick = static_cast<sf::Vector2f>(click) + cameraPos;
	while(tilemapClick.x < 0)
		tilemapClick.x += tilemapSize.x;
	while(tilemapClick.y < 0)
		tilemapClick.y += tilemapSize.y;
	while(tilemapClick.x > tilemapSize.x)
		tilemapClick.x -= tilemapSize.x;
	while(tilemapClick.y > tilemapSize.y)
		tilemapClick.y -= tilemapSize.y;
	for(auto& tile : myTiles)
	{
		auto tilePos = static_cast<sf::Vector2f>(tile.getPos());
		tilePos.x *= tileSize.x;
		tilePos.y *= tileSize.y;
		sf::FloatRect rect(tilePos, tileSize);
		if(rect.contains(tilemapClick))
		{
			return tile;
		}
	}
	return *myTiles.begin();
}

const Tile& Tilemap::getTile(const sf::Vector2i pos) const
{
	return myTiles[pos.x + pos.y * mySize.x];
}

void Tilemap::selectTile(sf::Vector2i pos)
{
	REQUIRES_P(pos.x >= 0 && pos.x < static_cast<int>(mySize.x));
	REQUIRES_P(pos.y >= 0 && pos.y < static_cast<int>(mySize.y));
	if(mySelectedTile.x >= 0 && mySelectedTile.y >= 0)
	{
		auto& prev = myTiles[mySelectedTile.x + mySelectedTile.y * mySize.x];
		auto color = prev.calculateColor(*this);
		auto cloud = findCloudSlowTest(mySelectedTile);
		if(cloud)
		{
			color.b = cloud->isPollution() ? 2 : 1;
		}
		else
		{
			color.b = 0;
		}
		myColorCache.setPixel(mySelectedTile.x, mySelectedTile.y, color);
	}
	mySelectedTile = pos;
	myColorCache.setPixel(pos.x, pos.y, TerrainPalette[0]);
	myIsDirty = true;
}

void Tilemap::resetSelected()
{
	if(mySelectedTile.x >= 0 && mySelectedTile.y >= 0)
	{
		auto& prev = myTiles[mySelectedTile.x + mySelectedTile.y * mySize.x];
		auto color = prev.calculateColor(*this);
		auto cloud = findCloudSlowTest(mySelectedTile);
		if(cloud)
		{
			color.b = cloud->isPollution() ? 2 : 1;
		}
		else
		{
			color.b = 0;
		}
		myColorCache.setPixel(mySelectedTile.x, mySelectedTile.y, color);
	}
	mySelectedTile = sf::Vector2i(-1, -1);
	myIsDirty = true;
}

void Tilemap::setBuilding(sf::Vector2i pos, const Building* building)
{
	REQUIRES_P(pos.x >= 0 && pos.x < static_cast<int>(mySize.x));
	REQUIRES_P(pos.y >= 0 && pos.y < static_cast<int>(mySize.y));
	auto& tile = myTiles[pos.x + pos.y * mySize.x];
	tile.setBuilding(building);
	myColorCache.setPixel(pos.x, pos.y, tile.calculateColor(*this));
	myIsDirty = true;
}

void Tilemap::setTileTerrain(sf::Vector2i pos, const std::string& terrain)
{
	REQUIRES_P(pos.x >= 0 && pos.x < static_cast<int>(mySize.x));
	REQUIRES_P(pos.y >= 0 && pos.y < static_cast<int>(mySize.y));
	const Terrain* wanted = nullptr;
	for(auto& t : myTerrains)
	{
		if(t->getName() == terrain)
		{
			wanted = t.get();
		}
	}
	if(wanted)
	{
		auto& tile = myTiles[pos.x + pos.y * mySize.x];
		tile.setTerrain(wanted);
		myColorCache.setPixel(pos.x, pos.y, tile.calculateColor(*this));
		myIsDirty = true;
	}
}

const Tile& Tilemap::findFirstConvertable(sf::Vector2i pos, const std::string& terrain, int nMaxCount) const
{
	const sf::Vector2i Dir[] = {
		sf::Vector2i(-1, 0),
		sf::Vector2i(1, 0),
		sf::Vector2i(0, 1),
		sf::Vector2i(0, -1),
	};
	int counter = 0;
	std::deque<sf::Vector2i> potentials;
	std::vector<sf::Vector2i> visited;
	potentials.push_back(pos);
	do {
		sf::Vector2i current = potentials.front();
		potentials.pop_front();
		visited.push_back(current);
		if(!isInside(current))
			continue;
			
		auto tile = getTile(current);
		if(tile.getTerrain()->canSwitchTo(terrain))
		{
			return getTile(current);
		}
		
		if(tile.getTerrain()->getName() == terrain)
		{
			for(auto dir : Dir)
			{
				auto neighbour = current + dir;
				if(!macros::contains(potentials, neighbour) && !macros::contains(visited, neighbour))
					potentials.push_back(neighbour);
			}
		}
	} while(counter++ < nMaxCount && !potentials.empty());
	return getTile(pos);
}

Path Tilemap::findFirstConvertableWalkable(sf::Vector2i pos, const std::string& terrain, int nMaxCount) const
{
	const sf::Vector2i Dir[] = {
		sf::Vector2i(-1, 0),
		sf::Vector2i(1, 0),
		sf::Vector2i(0, 1),
		sf::Vector2i(0, -1),
	};
	int counter = 0;
	std::deque<sf::Vector2i> potentials;
	std::vector<sf::Vector2i> visited;
	potentials.push_back(pos);
	do {
		sf::Vector2i current = potentials.front();
		potentials.pop_front();
		visited.push_back(current);
		if(!isInside(current))
			continue;
			
		auto tile = getTile(current);
		if(current != pos && tile.getTerrain()->canSwitchTo(terrain))
		{
			std::deque<sf::Vector2i> path;
			path.push_back(current);
			visited.pop_back();
			do {
				sf::Vector2i toConnect = path.front();
				int bestManhattan = 999999;
				sf::Vector2i bestNode = toConnect;
				for(auto node : visited)
				{
					bool isNeighbour = false;
					for(auto dir : Dir)
					{
						auto neighbour = node + dir;
						if(neighbour == toConnect)
						{
							isNeighbour = true;
							break;
						}
					}
					if(isNeighbour)
					{
						int manhattan = std::abs(node.x - pos.x) + std::abs(node.y - pos.y);
						if(manhattan < bestManhattan)
						{
							bestManhattan = manhattan;
							bestNode = node;
						}
					}
				}
				auto it = macros::find(visited, bestNode);
				if(it == visited.end())
					break;

				path.push_front(bestNode);
				visited.erase(it);
			} while(!macros::contains(path, pos));
			return Path(path);
		}
		
		if(!tile.getTerrain()->isImpassable())
		{
			for(auto dir : Dir)
			{
				auto neighbour = current + dir;
				if(!isInside(neighbour))
					continue;
				auto neighbourTile = getTile(neighbour);
				if(neighbourTile.getTerrain()->isImpassable())
					continue;
				if(!macros::contains(potentials, neighbour) && !macros::contains(visited, neighbour))
					potentials.push_back(neighbour);
			}
		}
	} while(counter++ < nMaxCount && !potentials.empty());
	std::deque<sf::Vector2i> path;
	return Path(path);
}

void Tilemap::applyFunctionInSphere(sf::Vector2i pos, int range, std::function<void(const Tile& tile)> functor) const
{
	const sf::Vector2i Dir[] = {
		sf::Vector2i(-1, 0),
		sf::Vector2i(1, 0),
		sf::Vector2i(0, 1),
		sf::Vector2i(0, -1),
	};
	int manhattan = 0;
	std::deque<sf::Vector2i> potentials;
	std::vector<sf::Vector2i> visited;
	potentials.push_back(pos);
	do {
		sf::Vector2i current = potentials.front();
		potentials.pop_front();
		visited.push_back(current);
		if(!isInside(current))
			continue;
			
		auto tile = getTile(current);
		functor(tile);
		manhattan = std::abs(current.x - pos.x) + std::abs(current.y - pos.y);
		
		for(auto dir : Dir)
		{
			auto neighbour = current + dir;
			if(!macros::contains(potentials, neighbour) && !macros::contains(visited, neighbour))
				potentials.push_back(neighbour);
		}
	} while(manhattan++ < range && !potentials.empty());
}

int Tilemap::countConvertable(sf::Vector2i pos, const std::string& terrain, int maxRange) const
{
	int count = 0;
	applyFunctionInSphere(pos, maxRange, [&count, &terrain](const Tile& tile){
		if(tile.getTerrain()->canSwitchTo(terrain))
			count++;
	});
	return count;
}

void Tilemap::createCloud(sf::Vector2i pos, int volume, bool pollution)
{
	REQUIRES_P(canHaveCloud(pos));
	auto cloud = std::make_unique<Cloud>(pos, volume, pollution, *this);
	cloud->applyToImage(myColorCache);
	myIsDirty = true;
	myClouds.push_back(std::move(cloud));
}

bool Tilemap::hasCloud(sf::Vector2i pos) const
{
	auto color = myColorCache.getPixel(pos.x, pos.y);
	return color.b != 0;
}

const Cloud* Tilemap::findCloudSlowTest(sf::Vector2i pos) const
{
	for(auto& cloud : myClouds)
	{
		for(auto part : cloud->getParts())
		{
			if(part == pos)
				return cloud.get();
		}
	}
	return nullptr;
}

bool Tilemap::canHaveCloud(sf::Vector2i pos) const
{
	if(hasCloud(pos))
		return false;

	auto tile = getTile(pos);
	if(!tile.getTerrain()->canHaveCloud())
		return false;
	return true;
}

const std::vector<Tile>& Tilemap::getTiles() const
{
	return myTiles;
}

bool Tilemap::isInside(sf::Vector2i pos) const
{
	return pos.x >= 0 && pos.y >= 0 && pos.x < static_cast<int>(mySize.x) && pos.y < static_cast<int>(mySize.y);
}

void Tilemap::toggleClouds()
{
	myShowClouds = !myShowClouds;
}

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	REQUIRES_P(states.shader != nullptr);
	
	sf::Vector2i targetSize = static_cast<sf::Vector2i>(target.getSize());
	sf::Vector2f textureStart(0, 0);
	sf::Vector2f textureEnd(1.0, 1.0);
	float offset = targetSize.y / 4;
	sf::VertexArray buffer(sf::TrianglesStrip, 4);
	buffer[0] = sf::Vertex(sf::Vector2f(0, offset), textureStart);
	buffer[1] = sf::Vertex(sf::Vector2f(targetSize.x, offset), sf::Vector2f(textureEnd.x, textureStart.y));
	buffer[2] = sf::Vertex(sf::Vector2f(0, targetSize.y), sf::Vector2f(textureStart.x, textureEnd.y));
	buffer[3] = sf::Vertex(sf::Vector2f(targetSize.x, targetSize.y), textureEnd);
	target.draw(buffer, states);
}

void Tilemap::setupTerrains()
{
	myTerrains.emplace_back(new Terrain("Wasteland", 1, true, false, 0.0,
		{"Power Plant", "Atmo Plant", "Scrubber", "Nuke", "Comet", "Terraformer"}, {"Barren"}
	));
	myTerrains.emplace_back(new Terrain("Ruins", 2, true, false, 0.0,
		{"Power Plant", "Atmo Plant", "Nuke", "Comet"}, { "Settlement" }
	));
	myTerrains.emplace_back(new Terrain("Glass", 3, true, false, 0.001,
		{"Power Plant", "Atmo Plant", "Nuke", "Comet", "Terraformer"}, {}
	));
	myTerrains.emplace_back(new Terrain("Mountains", 4, false, true, 0.0,
		{"Nuke", "Comet", "Terraformer"}, {}
	));
	myTerrains.emplace_back(new Terrain("Ocean Bed", 5, true, true, 0.0,
		{"Power Plant", "Atmo Plant", "Nuke", "Comet", "Terraformer"}, {"Water"}
	));
	myTerrains.emplace_back(new Terrain("Water", 6, true, true, 0.0,
		{"Water Plant", "Nuke", "Comet", "Terraformer"}, {"Ocean Bed"}
	));
	myTerrains.emplace_back(new Terrain("Barren", 7, true, false, 0.0,
		{"Power Plant", "Atmo Plant", "Scrubber", "Nuke", "Comet", "Terraformer"}, {"Restored", "Wasteland"}
	));
	myTerrains.emplace_back(new Terrain("Restored", 8, true, false, 0.0,
		{"Power Plant", "Atmo Plant", "Scrubber", "Nuke", "Comet", "Biomass Gen", "Fusion Plant", "Terraformer"}, {"Settlement", "Wasteland", "Biosphere"}
	));
	myTerrains.emplace_back(new Terrain("Settlement", 9, true, false, 0.0,
		{"Polluter", "Nuke", "Comet"}, {}
	));
	myTerrains.emplace_back(new Terrain("Biosphere", 10, true, false, 0.001,
		{"Power Plant", "Atmo Plant", "Scrubber", "Nuke", "Comet", "Biomass Gen", "Fusion Plant", "Terraformer"}, {"Settlement", "Wasteland"}
	));
}

void Tilemap::setupTiles(const sf::Image& source)
{
	auto wastelandTerrain = myTerrains[0].get();
	myTiles.reserve(mySize.x * mySize.y);
	for(unsigned int y = 0; y < mySize.y; y++)
	{
		for(unsigned int x = 0; x < mySize.x; x++)
		{
			auto terrain = findTerrainFromColor(source.getPixel(x, y));
			if(!terrain)
				terrain = wastelandTerrain;

			Tile tile(sf::Vector2i(x, y), terrain);
			myTiles.push_back(tile);
			myColorCache.setPixel(x, y, tile.calculateColor(*this));
		}
	}
	myIsDirty = true;
}

void Tilemap::updateTexture()
{
	if(myIsDirty)
	{
		myTexture.update(myColorCache);
		myIsDirty = false;
	}
}
