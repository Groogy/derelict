#pragma once

#include "tile.hpp"
#include "terrain.hpp"
#include "cloud.hpp"
#include "path.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <memory>

namespace sf
{
	class Event;
}

class Camera2D;
class Building;

class Tilemap : public sf::Drawable
{
public:
	Tilemap(const std::string& sourceFile);

	void update(Earth& earth);
	void updateTexture();

	sf::Vector2i getSize() const;

	const sf::Texture& getTexture() const;
	sf::Image& accessImage();
	void prepareShader(sf::Shader& shader, const Camera2D& camera, sf::Time timeSinceStart) const;

	const Terrain* findTerrainFromColor(sf::Color color) const;

	const Tile& findTileFromClick(const sf::Vector2i click, const Camera2D& camera) const;
	const Tile& getTile(const sf::Vector2i pos) const;

	void selectTile(sf::Vector2i pos);
	void resetSelected();
	void setBuilding(sf::Vector2i pos, const Building* building);
	void setTileTerrain(sf::Vector2i pos, const std::string& terrain);
	const Tile& findFirstConvertable(sf::Vector2i pos, const std::string& terrain, int nMaxCount = 999999999) const;
	Path findFirstConvertableWalkable(sf::Vector2i pos, const std::string& terrain, int nMaxCount = 999999999) const;
	void applyFunctionInSphere(sf::Vector2i pos, int range, std::function<void(const Tile& tile)> functor) const;
	int countConvertable(sf::Vector2i pos, const std::string& terrain, int maxRAnge = 999999999) const;

	void createCloud(sf::Vector2i pos, int volume, bool pollution);
	bool hasCloud(sf::Vector2i pos) const;
	const Cloud* findCloudSlowTest(sf::Vector2i pos) const;
	bool canHaveCloud(sf::Vector2i pos) const;

	const std::vector<Tile>& getTiles() const;

	bool isInside(sf::Vector2i pos) const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setupTerrains();
	void setupTiles(const sf::Image& source);

	sf::Vector2u mySize;
	std::vector<Tile> myTiles;
	std::vector<std::unique_ptr<Terrain>> myTerrains;
	std::vector<std::unique_ptr<Cloud>> myClouds;

	sf::Vector2i mySelectedTile = sf::Vector2i(-1, -1);

	sf::Image myColorCache;
	sf::Texture myTexture;
	bool myIsDirty = false;
};
