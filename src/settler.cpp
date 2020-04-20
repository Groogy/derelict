#include "settler.hpp"
#include "earth.hpp"
#include "tilemap.hpp"

#include <SFML/Graphics/Image.hpp>

Settler::Settler(Human& faction, sf::Vector2i pos, const Path& path)
: myFaction(faction)
, myPosition(pos)
, myPath(path)
{

}

sf::Vector2i Settler::getPosition() const
{
	return myPosition;
}

bool Settler::update(Earth& earth)
{
	auto& image = earth.accessTilemap().accessImage();
	clearFromImage(image);

	myPosition = myPath.popNext();
	if(myPath.isFinished())
		return true;
	
	applyToImage(image);
	return false;
}

void Settler::clearFromImage(sf::Image& colorCache) const
{
	auto color = colorCache.getPixel(myPosition.x, myPosition.y);
	color.a = 0;
	colorCache.setPixel(myPosition.x, myPosition.y, color);
}

void Settler::applyToImage(sf::Image& colorCache) const
{
	auto color = colorCache.getPixel(myPosition.x, myPosition.y);
	color.a = 1;
	colorCache.setPixel(myPosition.x, myPosition.y, color);
}
