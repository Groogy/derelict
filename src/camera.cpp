#include "camera.hpp"

#include <SFML/OpenGL.hpp>

void Camera::setSize(sf::Vector2u size)
{
	REQUIRES_P(size.y != 0);
	float aspect = static_cast<float>(size.x) / size.y;
	glViewport(0, 0, size.x, size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-aspect, aspect, -1.0, 1.0, 1.0, 5.0);
	mySize = size;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

sf::Vector2u Camera::getSize() const
{
	return mySize;
}

void Camera::setTransform(Transform transform)
{
	myTransform = transform;
}

Transform Camera::getTransform() const
{
	return myTransform;
}

void Camera::setOffset(float offset)
{
	myTransform.offset = offset;
}

float Camera::getOffset() const
{
	return myTransform.offset;
}

void Camera::setRotation(sf::Vector2f rotation)
{
	myTransform.rotation = rotation;
}

sf::Vector2f Camera::getRotation() const
{
	return myTransform.rotation;
}