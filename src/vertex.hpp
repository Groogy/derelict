#pragma once

#include <SFML/System/Vector3.hpp>
#include <vector>

struct Vertex
{
	Vertex(sf::Vector3f pos);

	sf::Vector3f position;
};

using IndexType = std::size_t;
