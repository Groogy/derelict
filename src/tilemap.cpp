#include "tilemap.hpp"
#include "camera2d.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

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

void Tilemap::update()
{
	updateTexture();
}

const sf::Texture& Tilemap::getTexture() const
{
	return myTexture;
}

void Tilemap::prepareShader(sf::Shader& shader, const Camera2D& camera) const
{
	shader.setUniform("terrainSampler", myTexture);
	shader.setUniform("cameraPosition", camera.getPosition());
	shader.setUniform("cameraZoom", camera.getZoom());
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

	sf::VertexArray line(sf::Lines, 4);
	line[0] = sf::Vertex(sf::Vector2f(targetSize.x / 2, offset));
	line[1] = sf::Vertex(sf::Vector2f(targetSize.x / 2, targetSize.y));
	line[2] = sf::Vertex(sf::Vector2f(0, (targetSize.y-offset) / 2 + offset));
	line[3] = sf::Vertex(sf::Vector2f(targetSize.x, (targetSize.y-offset) / 2 + offset));
	target.draw(line);
}

void Tilemap::setupTerrains()
{
	myTerrains.emplace_back(std::make_unique<Terrain>("Wasteland", sf::Color(255, 69, 0)));
	myTerrains.emplace_back(std::make_unique<Terrain>("Ruins", sf::Color(105, 105, 105)));
	myTerrains.emplace_back(std::make_unique<Terrain>("Glass", sf::Color(47, 79, 79)));
	myTerrains.emplace_back(std::make_unique<Terrain>("Mountains", sf::Color(50, 50, 50)));
	myTerrains.emplace_back(std::make_unique<Terrain>("Ocean Bed", sf::Color(188, 143, 143)));
}

void Tilemap::setupTiles(const sf::Image& source)
{
	auto wastelandTerrain = myTerrains[0].get();
	myTiles.reserve(mySize.x * mySize.y);
	for(unsigned int x = 0; x < mySize.x; x++)
	{
		for(unsigned int y = 0; y < mySize.y; y++)
		{
			auto terrain = findTerrainFromColor(source.getPixel(x, y));
			if(!terrain)
				terrain = wastelandTerrain;

			myTiles.emplace_back(terrain);
			myColorCache.setPixel(x, y, terrain->getColor());
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
