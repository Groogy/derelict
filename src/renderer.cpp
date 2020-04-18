#include "renderer.hpp"
#include "contracts.hpp"
#include "renderable.hpp"

#define GL_GLEXT_PROTOTYPES

#include <SFML/Graphics/Shader.hpp>
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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

const Camera3D& Renderer::getCamera() const
{
	return myCamera;
}

Camera3D& Renderer::accessCamera()
{
	return myCamera;
}

void Renderer::notifyResize(sf::Vector2u size)
{
	setActive();

	sf::Vector2i viewportSize = static_cast<sf::Vector2i>(size) / 4;
	sf::Vector2i viewportPosition = static_cast<sf::Vector2i>(size) - viewportSize;
	myCamera.setViewport(viewportPosition, viewportSize);
}

void Renderer::clear()
{
	setActive();
	glClearColor(myClearColor.r / 255.f, myClearColor.g / 255.f, myClearColor.b / 255.f, myClearColor.a / 255.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const std::vector<Vertex>& vertices, sf::PrimitiveType primitive, Transform transform, sf::Shader* shader)
{
	std::vector<IndexType> indices;
	indices.reserve(vertices.size());
	for(IndexType index = 0; index < vertices.size(); index++)
		indices.push_back(index);
	
	draw(vertices, indices, primitive, transform, shader);
}

void Renderer::draw(const std::vector<Vertex>& vertices, const std::vector<IndexType>& indices, sf::PrimitiveType primitive, Transform transform, sf::Shader* shader)
{
	setupShader(shader);

	glPushMatrix();
	applyInverseTransformation(myCamera.getTransform());
	applyTransformation(transform);
	glBegin(convertToOpenGLPrimitive(primitive));
	for(auto index : indices)
	{
		const auto& vertex = vertices[index];
		glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
		glTexCoord2f(vertex.texcoord.x, vertex.texcoord.y);
		//glNormal3f(vertex.normal.x, vertex.normal.y, vertex.normal.z);
	}
	glEnd();
	glPopMatrix();
}

void Renderer::draw(const sf::Drawable& drawable, sf::RenderStates states)
{
	myTarget.draw(drawable, states);
}

void Renderer::draw(const Renderable& renderable, sf::Shader* shader)
{
	draw(renderable.getVertices(), renderable.getIndices(), sf::Triangles, renderable.getTransform(), shader);
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

void Renderer::setupShader(sf::Shader* shader)
{
	sf::Shader::bind(shader);
	//int program = 0;
	//glGetIntegerv(GL_CURRENT_PROGRAM, &program);
	//locVertex = glGetAttribLocation(program, "position");
}
