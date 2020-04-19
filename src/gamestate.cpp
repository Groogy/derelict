#include "gamestate.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>

#include <SFML/OpenGL.hpp>

#include <cmath>

extern sf::Color TerrainPalette[];

GameState::GameState(sf::RenderWindow& window)
: myWindow(window)
, myRenderer(window)
{
	myRenderer.accessCamera().setOffset(14.1);
	myRenderer.accessCamera().setRotation(sf::Vector2f(0, -45.f));

	myEarthShader.loadFromFile("earth.vertex", "earth.fragment");
	myTilemapShader.loadFromFile("default.vertex", "tilemap.fragment");
	myEarthShader.setUniform("terrainSampler", myEarth.getTilemap().getTexture());

	myTopbar.setupBuildings();
	myTopbar.setupGui(myEarth, static_cast<sf::Vector2i>(myWindow.getSize()), myCamera);
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
	sf::Time delta = myClock.restart();
	myTimeSinceStart += delta;
	myTimeSinceLastTick += delta;
	handleEvents(delta);
	if(myTimeSinceLastTick > sf::seconds(1.0))
	{
		myTimeSinceLastTick = sf::Time::Zero;
		handleTick();
	}
	handleRender();
	handleUI();
}

void GameState::handleEvents(sf::Time delta)
{
	sf::Event event;
	while(myWindow.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
		{
			myWindow.close();
		}
		myInput.processEvent(event);
		myTopbar.processEvent(event);
	}

	float seconds = delta.asSeconds();
	myCamera.zoom(myInput.getCameraZoomChange() * seconds);
	auto movement = myInput.getCameraMovement();
	float zoom = myCamera.getZoom();
	movement *= zoom;
	myCamera.move(movement * seconds);

	auto& camera3d = myRenderer.accessCamera();
	camera3d.setRotation(camera3d.getRotation() + sf::Vector2f(4.0 * seconds, 0.0));
}

void GameState::handleTick()
{
	 myEarth.update();
	 myTopbar.update();
}

void GameState::handleRender()
{
	sf::Glsl::Vec4 Tmp[6];
	for(int index = 0; index < 6; index++)
	{
		sf::Color color = TerrainPalette[index];
		Tmp[index] = sf::Glsl::Vec4(
			static_cast<float>(color.r) / 256, 
			static_cast<float>(color.g) / 256, 
			static_cast<float>(color.b) / 256, 
			static_cast<float>(color.a) / 256
		);
	}
	myEarthShader.setUniformArray("colorPalette", Tmp, 6);

	myEarth.accessTilemap().updateTexture();
	myRenderer.clear();
	myRenderer.draw(myEarth, &myEarthShader);

	sf::Shader::bind(nullptr);
}

void GameState::handleUI()
{
	RenderStackState state(myRenderer);

	auto& tilemap = myEarth.getTilemap();
	tilemap.prepareShader(myTilemapShader, myCamera, myTimeSinceStart);
	myRenderer.draw(tilemap, &myTilemapShader);

	myTopbar.render(myRenderer);
}

