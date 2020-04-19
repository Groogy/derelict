#include "camera2d.hpp"
#include <algorithm>

sf::FloatRect Camera2D::getViewport() const
{
	return myViewport;
}

void Camera2D::setViewport(sf::FloatRect rect)
{
	myViewport = rect;
}

sf::Vector2f Camera2D::getPosition() const
{
	return myPosition;
}

void Camera2D::setPosition(sf::Vector2f pos)
{
	myPosition = pos;
}

void Camera2D::move(sf::Vector2f delta)
{
	myPosition += delta;
}

float Camera2D::getZoom() const
{
	return myZoom;
}

void Camera2D::setZoom(float value)
{
	//REQUIRES_P(value >= 0.05 && myZoom <= 0.2);
	myZoom = value;
}

void Camera2D::zoom(float delta)
{
	myZoom = std::clamp(myZoom + delta, 0.05f, 0.2f);
}
