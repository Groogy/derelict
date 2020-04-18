#include "renderer.hpp"
#include "contracts.hpp"
#include "renderable.hpp"

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
	notifyResize(target.getSize());
	float ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float position[] = { 5.0, 5.0, 5.0, 0.0 };

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	glEnable(GL_LIGHT0);
	//glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
}

const Camera& Renderer::getCamera() const
{
	return myCamera;
}

Camera& Renderer::accessCamera()
{
	return myCamera;
}

void Renderer::notifyResize(sf::Vector2u size)
{
	setActive();
	myCamera.setSize(size);
}

void Renderer::clear()
{
	setActive();
	glClearColor(myClearColor.r / 255.f, myClearColor.g / 255.f, myClearColor.b / 255.f, myClearColor.a / 255.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const std::vector<Vertex>& vertices, sf::PrimitiveType primitive, Transform transform)
{
	std::vector<IndexType> indices;
	indices.reserve(vertices.size());
	for(IndexType index = 0; index < vertices.size(); index++)
		indices.push_back(index);
	
	draw(vertices, indices, primitive, transform);
}

void Renderer::draw(const std::vector<Vertex>& vertices, const std::vector<IndexType>& indices, sf::PrimitiveType primitive, Transform transform)
{
	glPushMatrix();
	applyInverseTransformation(myCamera.getTransform());
	applyTransformation(transform);
	glBegin(convertToOpenGLPrimitive(primitive));
	for(auto index : indices)
	{
		const auto& vertex = vertices[index];
		glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
		glTexCoord2f(vertex.texcoord.x, vertex.texcoord.y);
		glNormal3f(vertex.normal.x, vertex.normal.y, vertex.normal.z);
	}
	glEnd();
	glPopMatrix();
}

void Renderer::draw(const sf::Drawable& drawable)
{
	myTarget.draw(drawable);
}

void Renderer::draw(const Renderable& renderable)
{
	draw(renderable.getVertices(), renderable.getIndices(), sf::Triangles, renderable.getTransform());
}

void Renderer::setActive()
{
	myTarget.setActive(true);
}

void Renderer::applyTransformation(Transform transform)
{
	glRotatef(transform.rotation.x, 1.0, 0.0, 0.0);
	glRotatef(transform.rotation.y, 0.0, 1.0, 0.0);
	glTranslatef(0, 0, -transform.offset);
}

void Renderer::applyInverseTransformation(Transform transform)
{
	glTranslatef(0, 0, -transform.offset);
	glRotatef(transform.rotation.y, 1.0, 0.0, 0.0);
	glRotatef(transform.rotation.x, 0.0, 0.0, 1.0);
}
