#include "renderer.hpp"
#include "contracts.hpp"

#include <SFML/OpenGL.hpp>

namespace
{
	GLenum convertToOpenGLPrimitive(sf::PrimitiveType primitive)
	{
		REQUIRES_P(primitive >= sf::Points && primitive <= sf::Quads);
		switch(primitive)
		{
			case sf::Points: return GL_POINTS;
			case sf::Lines: return GL_LINES;
			case sf::LineStrip: return GL_LINE_STRIP;
			case sf::Triangles: return GL_TRIANGLES;
			case sf::TriangleStrip: return GL_TRIANGLE_STRIP;
			case sf::TriangleFan: return GL_TRIANGLE_FAN;
			case sf::Quads: return GL_QUADS;
		}
		return GL_POINTS;
	}
}

RenderStackState::RenderStackState(Renderer& renderer)
: myRenderer(renderer)
{
	myRenderer.myTarget.pushGLStates();
}
RenderStackState::~RenderStackState()
{
	myRenderer.myTarget.popGLStates();
}

Renderer::Renderer(sf::RenderTarget& target)
: myTarget(target)
{

}

void Renderer::clear()
{
	setActive();
	glClearColor(myClearColor.r / 255.f, myClearColor.g / 255.f, myClearColor.b / 255.f, myClearColor.a / 255.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const std::vector<Vertex>& vertices, sf::PrimitiveType primitive)
{
	std::vector<IndexType> indices;
	indices.reserve(vertices.size());
	for(IndexType index = 0; index < vertices.size(); index++)
		indices.push_back(index);
	
	draw(vertices, indices, primitive);
}

void Renderer::draw(const std::vector<Vertex>& vertices, const std::vector<IndexType>& indices, sf::PrimitiveType primitive)
{
	glBegin(convertToOpenGLPrimitive(primitive));
	for(auto index : indices)
	{
		const auto& vertex = vertices[index];
		glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
	}
	glEnd();
}

void Renderer::draw(const sf::Drawable& drawable)
{
	myTarget.draw(drawable);
}

void Renderer::setActive()
{
	myTarget.setActive(true);
}