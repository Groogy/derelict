#include "gamestate.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>

#include <SFML/OpenGL.hpp>

#include <cmath>

GameState::GameState(sf::RenderWindow& window)
: myWindow(window)
, myRenderer(window)
{
	myRenderer.accessCamera().setOffset(14);
	myRenderer.accessCamera().setRotation(sf::Vector2f(0, -45.f));

	myEarthShader.loadFromFile("earth.vertex", "earth.fragment");
	myTilemapShader.loadFromFile("default.vertex", "tilemap.fragment");
	myEarthShader.setUniform("terrainSampler", myEarth.getTilemap().getTexture());
}

GameState::~GameState()
{
}

bool GameState::isRunning() const
{
	return myWindow.isOpen();
}

void GameState::update()
{
	handleEvents();
	if(myClock.getElapsedTime() > sf::milliseconds(20))
	{
		myClock.restart();
		handleTick();
	}
	handleRender();
	handleUI();
}

void GameState::handleEvents()
{
	sf::Event event;
	while(myWindow.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
		{
			myWindow.close();
		}
		myInput.processEvent(event);
	}
}

void GameState::handleTick()
{
	myCamera.zoom(myInput.getCameraZoomChange());
	auto movement = myInput.getCameraMovement();
	float zoom = myCamera.getZoom();
	movement *= zoom;
	myCamera.move(movement);

	auto& camera3d = myRenderer.accessCamera();
	camera3d.setRotation(camera3d.getRotation() + sf::Vector2f(0.2, 0.0));
}

void GameState::handleRender()
{
	myRenderer.clear();
	myRenderer.draw(myEarth, &myEarthShader);

	sf::Shader::bind(nullptr);
}

void GameState::handleUI()
{
	RenderStackState state(myRenderer);
	sf::RectangleShape shape;
	shape.setFillColor(sf::Color::Yellow);
	shape.setSize(sf::Vector2f(100, 100));

	myRenderer.draw(shape);

	auto& tilemap = myEarth.getTilemap();
	tilemap.prepareShader(myTilemapShader, myCamera);
	myRenderer.draw(tilemap, &myTilemapShader);

	auto windowSize = myWindow.getSize();
	sf::VertexArray line(sf::Lines, 4);
	float width = windowSize.x / 8;
	float height = windowSize.y / 8;
	line[0] = sf::Vertex(sf::Vector2f(windowSize.x - width, 0));
	line[1] = sf::Vertex(sf::Vector2f(windowSize.x - width, height*2));
	line[2] = sf::Vertex(sf::Vector2f(windowSize.x - width*2, height));
	line[3] = sf::Vertex(sf::Vector2f(windowSize.x, height));
	myRenderer.draw(line);
}

