#include "gamestate.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>

#include "tilemap.hpp"

#include <SFML/OpenGL.hpp>

#include <cmath>

extern std::vector<sf::Color>  TerrainPalette;

GameState::GameState(sf::RenderWindow& window)
: myWindow(window)
, myRenderer(window)
{
	myRenderer.accessCamera().setOffset(14.1);

	myEarthShader.loadFromFile("earth.vertex", "earth.fragment");
	myTilemapShader.loadFromFile("default.vertex", "tilemap.fragment");
	myEarthShader.setUniform("terrainSampler", myEarth.getTilemap().getTexture());

	myTopbar.setupBuildings();
	myTopbar.setupGui(myEarth, static_cast<sf::Vector2i>(myWindow.getSize()), myCamera);

	myFont.loadFromFile("default.ttf");
	myNotification.setFont(myFont);
	myNotification.setPosition(10, 400);
	myNotification.setFillColor(sf::Color::Red);
	myNotification.setOutlineThickness(1);
	myNotification.setCharacterSize(36);
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
	if(myNotificationTimer > sf::Time::Zero)
		myNotificationTimer -= delta;

	handleEvents(delta);
	if(myTimeSinceLastTick > sf::seconds(1.0))
	{
		myTimeSinceLastTick = sf::Time::Zero;
		handleTick();
	}
	handleRender(delta);
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
}

void GameState::handleTick()
{
	if(myEarth.getHomeostasis().getValue() > 35.f)
	{
		if(myEarth.hasHumans())
		{
			if(rand() % 300 == 0)
			{
				handleHumanAppearance();
			}
		}
		else
		{
			handleHumanAppearance();
		}
	}
	if(rand() % 500000 == 0)
	{
		handleCometSighted();
	}
	 myEarth.update();
	 myTopbar.update();
}

void GameState::handleRender(sf::Time delta)
{
	std::vector<sf::Glsl::Vec4> tmp;
	for(unsigned int index = 0; index < TerrainPalette.size(); index++)
	{
		sf::Color color = TerrainPalette[index];
		tmp.emplace_back(
			static_cast<float>(color.r) / 256, 
			static_cast<float>(color.g) / 256, 
			static_cast<float>(color.b) / 256, 
			static_cast<float>(color.a) / 256
		);
	}
	myEarthShader.setUniformArray("colorPalette", &tmp[0], tmp.size());

	myEarth.accessTilemap().updateTexture();
	myRenderer.clear();

	auto& camera3d = myRenderer.accessCamera();

	auto targetSize = myWindow.getSize();
	sf::Vector2i viewportSize = static_cast<sf::Vector2i>(targetSize) / 4;
	sf::Vector2i viewportPosition = static_cast<sf::Vector2i>(targetSize) - viewportSize;
	camera3d.setViewport(viewportPosition, viewportSize);	
	float xRotation = camera3d.getRotation().x + 4.0 * delta.asSeconds();
	myRenderer.accessCamera().setRotation(sf::Vector2f(xRotation, -45.f));
	myRenderer.draw(myEarth, &myEarthShader);

	viewportPosition.x -= viewportSize.x;
	camera3d.setViewport(viewportPosition, viewportSize);
	myRenderer.accessCamera().setRotation(sf::Vector2f(xRotation, -135.f));
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
	if(myNotificationTimer > sf::Time::Zero)
		myRenderer.draw(myNotification);
}

void GameState::handleHumanAppearance()
{
	const Building* polluter = nullptr;
	for(auto& building : myTopbar.getBuildings())
	{
		if(building->getName() == "Polluter")
		{
			polluter = building.get();
			break;
		}
	}

	auto& tilemap = myEarth.accessTilemap();
	std::vector<sf::Vector2i> potentials;
	for(auto tile : tilemap.getTiles())
	{
		if(tile.getTerrain()->getName() == "Ruins")
		{
			int nCount = tilemap.countConvertable(tile.getPos(), "Settlement", 16);
			if(nCount <= 10)
				continue;
				
			int weight = nCount;
			for(int i = 0; i < weight; i++)
				potentials.push_back(tile.getPos());
		}
	}
	if(potentials.empty())
		return;

	int random = rand() % potentials.size();
	auto pos = potentials[random];
	myEarth.spawnHuman(pos, polluter);

	myNotification.setString("HUMANS HAVE FOUNDED A SETTLEMENT!");
	myNotificationTimer = sf::seconds(5.f);
}

void GameState::handleCometSighted()
{
	auto& tilemap = myEarth.accessTilemap();
	int randomX = rand() % tilemap.getSize().x;
	int randomY = rand() % tilemap.getSize().y;

	const Building* comet = nullptr;
	for(auto& building : myTopbar.getBuildings())
	{
		if(building->getName() == "Comet")
		{
			comet = building.get();
			break;
		}
	}

	tilemap.setBuilding(sf::Vector2i(randomX, randomY), comet);

	myNotification.setString("COMET SIGHTED -1 STABILITY");
	myNotificationTimer = sf::seconds(5.f);
}
