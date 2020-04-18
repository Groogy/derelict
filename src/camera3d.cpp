#include "camera3d.hpp"

#include <SFML/OpenGL.hpp>

#include <cmath>

void Camera3D::setViewport(sf::Vector2i pos, sf::Vector2i size)
{
	constexpr float Pi = 3.141592654f;

	REQUIRES_P(size.y != 0);
	float aspect = static_cast<float>(size.x) / size.y;
	float fov = std::tan(90.0 * Pi / 360.0);
	glViewport(pos.x, pos.y, size.x, size.y);
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

sf::Vector2i Camera3D::getSize() const
{
	return mySize;
}

void Camera3D::setTransform(Transform transform)
{
	myTransform = transform;
}

Transform Camera3D::getTransform() const
{
	return myTransform;
}

void Camera3D::setOffset(float offset)
{
	myTransform.offset = offset;
}

float Camera3D::getOffset() const
{
	return myTransform.offset;
}

void Camera3D::setRotation(sf::Vector2f rotation)
{
	myTransform.rotation = rotation;
}

sf::Vector2f Camera3D::getRotation() const
{
	return myTransform.rotation;
}
