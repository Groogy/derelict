#pragma once

#include "contracts.hpp"
#include "transform.hpp"

#include <SFML/System/Vector2.hpp>

class Camera
{
public:
	void setSize(sf::Vector2u size);
	sf::Vector2u getSize() const;

	void setTransform(Transform transform);
	Transform getTransform() const;

	void setOffset(float offset);
	float getOffset() const;

	void setRotation(sf::Vector2f rotation);
	sf::Vector2f getRotation() const;

private:
	sf::Vector2u mySize;
	Transform myTransform;
};
