#include "gamestate.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>

GameState::GameState(sf::RenderWindow& window)
: myWindow(window)
, myRenderer(window)
{
	myRenderer.accessCamera().setOffset(11);

	myEarthShader.loadFromFile("earth.vertex", "earth.fragment");
	myEarthTexture.loadFromFile("earth.png");
	myEarthShader.setUniform("terrainSampler", myEarthTexture);
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
	auto transform = myRenderer.getCamera().getTransform();
	transform.offset += myInput.getCameraOffsetChange();
	transform.rotation += myInput.getCameraRotationChange();
	myRenderer.accessCamera().setTransform(transform);
}

void GameState::handleRender()
{
	myRenderer.clear();
	myRenderer.draw(myEarth, &myEarthShader);
}

void GameState::handleUI()
{
	RenderStackState state(myRenderer);
	sf::RectangleShape shape;
	shape.setFillColor(sf::Color::Yellow);
	shape.setSize(sf::Vector2f(100, 100));

	myRenderer.draw(shape);
}
