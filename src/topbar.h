#pragma once

#include "guiobject.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <vector>
#include <memory>

class Earth;
class Renderer;

class Topbar
{
public:
	void setup(Earth& earth);

	void update();
	void processEvent(const sf::Event& event);
	void render(Renderer& renderer) const;

private:
	std::vector<std::unique_ptr<GuiObject>> myObjects;
	std::vector<std::unique_ptr<sf::Sprite>> myIcons;
	std::vector<std::unique_ptr<sf::Texture>> myTextures;
};
