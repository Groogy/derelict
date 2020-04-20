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
	myText.setFillColor(sf::Color::Yellow);
	myText.setOutlineColor(sf::Color::Black);
	myText.setOutlineThickness(1.f);
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
		case sf::Event::MouseMoved:
			handleMouseOver(event);
			break;
		default:
			break;
	}
}

void Button::attachOnClickHook(OnClickHook hook)
{
	myOnClickHooks.push_back(hook);
}

void Button::attachOnHoverHook(OnHoverHook hook)
{
	myOnHoverHooks.push_back(hook);
}

sf::IntRect Button::getRect() const
{
	return sf::IntRect(
		static_cast<sf::Vector2i>(getPosition()), 
		static_cast<sf::Vector2i>(mySprite.getTexture()->getSize())
	);
}

void Button::setupFont()
{
	if(ourFont == nullptr)
	{
		ourFont = std::make_unique<sf::Font>();
		ourFont->loadFromFile("resources/default.ttf");
	}
}

const sf::Font& Button::getFont()
{
	ENSURES_P(ourFont != nullptr);
	return *ourFont;
}

void Button::handleMouseButtonPress(const sf::Event& event)
{
	if(getRect().contains(event.mouseButton.x, event.mouseButton.y))
	{
		onClick();
	}
}

void Button::handleMouseOver(const sf::Event& event)
{
	if(getRect().contains(event.mouseMove.x, event.mouseMove.y))
	{
		onHover();
	}
}

void Button::onClick()
{
	for(auto& hook : myOnClickHooks)
	{
		hook();
	}
}

void Button::onHover()
{
	for(auto& hook : myOnHoverHooks)
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

