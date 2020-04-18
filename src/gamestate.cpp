#include "gamestate.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

GameState::GameState(sf::RenderWindow& window)
: myWindow(window)
, myRenderer(window)
{
	myRenderer.accessCamera().setOffset(2);
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
	handleTick();
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
	transform.offset += myInput.getCameraOffsetChange() / 1000;
	transform.rotation += myInput.getCameraRotationChange() * 0.01f;
	myRenderer.accessCamera().setTransform(transform);
}

void GameState::handleRender()
{
	myRenderer.clear();
	myRenderer.draw(myEarth);
}

void GameState::handleUI()
{
	RenderStackState state(myRenderer);
	sf::RectangleShape shape;
	shape.setFillColor(sf::Color::Yellow);
	shape.setSize(sf::Vector2f(100, 100));

	myRenderer.draw(shape);
}
