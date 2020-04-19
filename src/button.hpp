#pragma once

#include "guiobject.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <functional>
#include <memory>
#include <string>

namespace sf
{
	class Event;
}

class Button : public GuiObject
{
public:
	using OnClickHook = std::function<void()>;
	Button(const sf::Texture& texture, const std::string& text);

	virtual void processEvent(const sf::Event& event) override;

	void attachOnClickHook(OnClickHook hook);

private:
	static void setupFont();
	static const sf::Font& getFont();

	void handleMouseButtonPress(const sf::Event& event);
	void onClick();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	static std::unique_ptr<sf::Font> ourFont;

	sf::Text myText;
	sf::Sprite mySprite;

	std::vector<OnClickHook> myOnClickHooks;
};
