#include "path.hpp"

Path::Path(const std::deque<sf::Vector2i>& path)
: myNodes(path)
{
}

int Path::getSize() const
{
	return myNodes.size();
}

bool Path::isFinished() const
{
	return myNodes.empty();
}

sf::Vector2i Path::popNext()
{
	sf::Vector2i front = myNodes.front();
	myNodes.pop_front();
	return front;
}
