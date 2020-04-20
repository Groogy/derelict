#pragma once

#include "renderer.hpp"
#include "earth.hpp"
#include "player_input.hpp"
#include "camera2d.hpp"
#include "topbar.h"
#include "state.hpp"

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/Music.hpp>
namespace sf
{
	class RenderWindow;
}

class GameState : public State
{
public:
	GameState(sf::RenderWindow& window);
	~GameState();

	bool isRunning() const;

	virtual State* update() override;

private:
	void handleEvents(sf::Time delta);
	void handleTick();
	void handleRender(sf::Time delta);
	void handleUI();


	void handleHumanAppearance();
	void handleCometSighted();

	sf::RenderWindow& myWindow;
	sf::Clock myClock;
	sf::Time myTimeSinceLastTick;
	sf::Time myTimeSinceStart;
	Renderer myRenderer;
	PlayerInput myInput;
	Earth myEarth;

	Camera2D myCamera;

	sf::Shader myEarthShader;
	sf::Shader myTilemapShader;

	Topbar myTopbar;

	sf::Text myNotification;
	sf::Font myFont;
	sf::Time myNotificationTimer;

	sf::SoundBuffer myHumanSpawnSoundBuffers[4];
	sf::Sound myHumanSpawnSounds[4];

	bool myHumansHaveSpawned = false;
	State* myTempWinState = nullptr;
	bool myHasDecidedToContinue = false;
};
