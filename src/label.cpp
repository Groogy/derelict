#include "label.hpp"
#include "contracts.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

std::unique_ptr<sf::Font> Label::ourFont = nullptr;

Label::Label()
{
	setupFont();
	myText.setFont(getFont());
	myText.setCharacterSize(14);
}

void Label::setString(const std::string& text)
{
	myText.setString(text);
}

void Label::update()
{
	GuiObject::update();
	if(myTextChangeHook)
	{
		std::string tmp = myTextChangeHook();
		if(tmp != myText.getString())
		{
			myText.setString(tmp);
		}
	}
}

void Label::attachTextChangeHook(TextChangeHook hook)
{
	myTextChangeHook = hook;
}

void Label::setupFont()
{
	if(ourFont == nullptr)
	{
		ourFont = std::make_unique<sf::Font>();
		ourFont->loadFromFile("default.ttf");
	}
}

const sf::Font& Label::getFont()
{
	ENSURES_P(ourFont != nullptr);
	return *ourFont;
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(myText, states);
}


