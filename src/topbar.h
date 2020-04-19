#pragma once

#include "guiobject.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <vector>
#include <memory>

class Earth;
class Renderer;
class Camera2D;

class Topbar
{
public:
	void setup(Earth& earth, sf::Vector2i targetSize, const Camera2D& camera);

	void update();
	void processEvent(const sf::Event& event);
	void render(Renderer& renderer) const;

private:
	void handleOnTilemapClick(const sf::Event& event);

	std::function<bool(const sf::Event&)> myTilemapClickTest;
	std::function<void(const sf::Event&)> myTilemapClickInspect;
	std::function<void(const sf::Event&)> myTilemapClickAction;

	std::vector<std::unique_ptr<GuiObject>> myObjects;
	std::vector<std::unique_ptr<sf::Sprite>> myIcons;
	std::vector<std::unique_ptr<sf::Texture>> myTextures;

	std::string myTileInfoStr;
};
