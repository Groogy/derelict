#pragma once

#include <SFML/System/Vector2.hpp>

#include <deque>

class Path
{
public:
	Path(const std::deque<sf::Vector2i>& path);

	int getSize() const;
	bool isFinished() const;
	
	sf::Vector2i popNext();
private:
	std::deque<sf::Vector2i> myNodes;
};
