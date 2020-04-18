#pragma once

#include "vertex.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>

class Renderer;

struct RenderStackState
{
public:
	RenderStackState(Renderer& renderer);
	~RenderStackState();

private:
	Renderer& myRenderer;
};

class Renderer
{
public:
	Renderer(sf::RenderTarget& target);

	void clear();
	void draw(const std::vector<Vertex>& vertices, sf::PrimitiveType primitive);
	void draw(const std::vector<Vertex>& vertices, const std::vector<IndexType>& indices, sf::PrimitiveType primitive);
	void draw(const sf::Drawable& drawable);
	
private:
	friend class RenderStackState;

	void setActive();

	sf::RenderTarget& myTarget;
	sf::Color myClearColor = sf::Color::Black;
};
