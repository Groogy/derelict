#pragma once

#include "vertex.hpp"
#include "camera.hpp"
#include "transform.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>

class Renderer;
class Renderable;

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

	const Camera& getCamera() const;
	Camera& accessCamera();

	void notifyResize(sf::Vector2u size);

	void clear();
	void draw(const std::vector<Vertex>& vertices, sf::PrimitiveType primitive, Transform transform = Transform::Identity, sf::Shader* shader = nullptr);
	void draw(const std::vector<Vertex>& vertices, const std::vector<IndexType>& indices, sf::PrimitiveType primitive, Transform transform = Transform::Identity, sf::Shader* shader = nullptr);
	void draw(const sf::Drawable& drawable);
	void draw(const Renderable& renderable, sf::Shader* shader = nullptr);
	
private:
	friend class RenderStackState;

	void setActive();
	void applyTransformation(Transform transform);
	void applyInverseTransformation(Transform transform);

	void setupShader(sf::Shader* shader);

	sf::RenderTarget& myTarget;
	Camera myCamera;
	sf::Color myClearColor = sf::Color::Black;
};
