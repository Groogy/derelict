#pragma once

#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

struct Vertex
{
	Vertex(sf::Vector3f pos, sf::Vector2f uv, sf::Vector3f nor);

	sf::Vector3f position;
	sf::Vector2f texcoord;
	sf::Vector3f normal;
};

using IndexType = std::size_t;
