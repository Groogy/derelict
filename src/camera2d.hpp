#pragma once

#include "contracts.hpp"
#include "transform.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

class Camera2D
{
public:
	sf::FloatRect getViewport() const;
	void setViewport(sf::FloatRect rect);

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f pos);
	void move(sf::Vector2f delta);

	float getZoom() const;
	void setZoom(float value);
	void zoom(float delta);

private:
	sf::FloatRect myViewport;
	sf::Vector2f myPosition;
	float myZoom = 0.2;

};
