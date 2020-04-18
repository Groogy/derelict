#include "camera.hpp"

#include <SFML/OpenGL.hpp>

#include <cmath>

void Camera::setSize(sf::Vector2u size)
{
	constexpr float Pi = 3.141592654f;

	REQUIRES_P(size.y != 0);
	float aspect = static_cast<float>(size.x) / size.y;
	float fov = std::tan(90.0 * Pi / 360.0);
	glViewport(0, 0, size.x, size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float znear = 0.1;
	float zfar = 20.0;
    float ymax = znear * fov;
    float xmax = ymax * aspect;
	glFrustum(-xmax, xmax, -ymax, ymax, znear, zfar);
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