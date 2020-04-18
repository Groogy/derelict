#include "camera2d.hpp"

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
	myZoom = value;
}

void Camera2D::zoom(float delta)
{
	myZoom += delta;
}
