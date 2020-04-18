#include "contracts.hpp"
#include "renderer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics.hpp>

#include <cmath>

void generateSphere(std::vector<Vertex>& vertices, std::vector<IndexType>& indices, float radius)
{
	constexpr float Pi = 3.141592654f;
	constexpr int SectorCount = 64;
	constexpr int StackCount = 64;
	constexpr float SectorStep = 2 * Pi / SectorCount;
	constexpr float StackStep = Pi / StackCount;
	
	for(int stack = 0; stack <= StackCount; stack++)
	{
		float stackAngle = Pi / 2 - stack * StackStep;
		float xy = radius * std::cos(stackAngle);
		float z = radius * std::sin(stackAngle);
		for(int sector = 0; sector <= SectorCount; sector++)
		{
			float sectorAngle = sector * SectorStep;
			float x = xy * std::cos(sectorAngle);
			float y = xy * std::sin(sectorAngle);
			vertices.emplace_back(sf::Vector3f(x, y, z));
		}
	}

	for(int stack = 0; stack < StackCount; stack++)
	{
		int k1 = stack * (SectorCount + 1);
		int k2 = k1 + SectorCount + 1;
		for(int sector = 0; sector < SectorCount; sector++, k1++, k2++)
		{
			if(stack != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if(stack != (StackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
}

void draw3D(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<IndexType>& indices)
{
	renderer.draw(vertices, indices, sf::Triangles);
}

void draw2D(Renderer& renderer)
{
	RenderStackState state(renderer);
	sf::RectangleShape shape;
	shape.setFillColor(sf::Color::Yellow);
	shape.setSize(sf::Vector2f(100, 100));

	renderer.draw(shape);
}

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;

	std::vector<Vertex> sphere;
	std::vector<IndexType> indices;
	generateSphere(sphere, indices, 1.0);

	sf::RenderWindow window(sf::VideoMode(1280, 800), "Ludum Dare 46", sf::Style::Default, settings);
	Renderer renderer(window);
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		renderer.clear();

		draw3D(renderer, sphere, indices);
		draw2D(renderer);
		

		window.display();
	}
	return 0;
}