#pragma once

#include "guiobject.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include <functional>
#include <memory>

class Label : public GuiObject
{
public:
	using TextChangeHook = std::function<std::string()>;

	Label();

	void setString(const std::string& text);

	virtual void update() override;

	void attachTextChangeHook(TextChangeHook hook);

private:
	static void setupFont();
	static const sf::Font& getFont();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	static std::unique_ptr<sf::Font> ourFont;

	sf::Text myText;
	TextChangeHook myTextChangeHook;

};
