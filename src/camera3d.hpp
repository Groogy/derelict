#pragma once

#include "contracts.hpp"
#include "transform.hpp"

#include <SFML/System/Vector2.hpp>

class Camera3D
{
public:
	void setViewport(sf::Vector2i pos, sf::Vector2i size);

	sf::Vector2i getSize() const;

	void setTransform(Transform transform);
	Transform getTransform() const;

	void setOffset(float offset);
	float getOffset() const;

	void setRotation(sf::Vector2f rotation);
	sf::Vector2f getRotation() const;

private:
	sf::Vector2i mySize;
	Transform myTransform;
};
