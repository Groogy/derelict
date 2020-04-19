#include "button.hpp"
#include "contracts.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

std::unique_ptr<sf::Font> Button::ourFont = nullptr;

Button::Button(const sf::Texture& texture, const std::string& text)
{
	mySprite.setTexture(texture);

	setupFont();
	myText.setFont(getFont());
	myText.setCharacterSize(10);
	myText.setString(text);
	myText.setPosition(5, texture.getSize().y - 14);
}

void Button::processEvent(const sf::Event& event)
{
	switch(event.type)
	{
		case sf::Event::MouseButtonReleased:
			handleMouseButtonPress(event);
			break;
		default:
			break;
	}
}

void Button::attachOnClickHook(OnClickHook hook)
{
	myOnClickHooks.push_back(hook);
}

void Button::setupFont()
{
	if(ourFont == nullptr)
	{
		ourFont = std::make_unique<sf::Font>();
		ourFont->loadFromFile("default.ttf");
	}
}

const sf::Font& Button::getFont()
{
	ENSURES_P(ourFont != nullptr);
	return *ourFont;
}

void Button::handleMouseButtonPress(const sf::Event& event)
{
	auto& mouseButton = event.mouseButton;
	sf::IntRect rect(
		static_cast<sf::Vector2i>(getPosition()), 
		static_cast<sf::Vector2i>(mySprite.getTexture()->getSize())
	);
	if(rect.contains(mouseButton.x, mouseButton.y))
	{
		onClick();
	}
}

void Button::onClick()
{
	for(auto& hook : myOnClickHooks)
	{
		hook();
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mySprite, states);
	target.draw(myText, states);
}

