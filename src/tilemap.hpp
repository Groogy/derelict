#pragma once

#include "tile.hpp"
#include "terrain.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <vector>
#include <memory>

class Camera2D;

class Tilemap : public sf::Drawable
{
public:
	Tilemap(const std::string& sourceFile);

	void update();

	const sf::Texture& getTexture() const;
	void prepareShader(sf::Shader& shader, const Camera2D& camera) const;

	const Terrain* findTerrainFromColor(sf::Color color) const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setupTerrains();
	void setupTiles(const sf::Image& source);

	void updateTexture();

	sf::Vector2u mySize;
	std::vector<Tile> myTiles;
	std::vector<std::unique_ptr<Terrain>> myTerrains;

	sf::Image myColorCache;
	sf::Texture myTexture;
	bool myIsDirty = false;
};
