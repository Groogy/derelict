#pragma once

#include "state.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <sstream>

namespace sf
{
	class RenderWindow;
}

class GameOverState : public State
{
public:
	enum Condition
	{
		LossEnergy,
		LossHomeostasis,
		VictoryNoHumans,
		VictoryHumans
	};
	
	GameOverState(sf::RenderWindow& window, Condition condition, State* parent = nullptr);

	virtual State* update() override;

private:
	void updateText();

	sf::RenderWindow& myWindow;
	bool myStartGame = false;
	bool myContinueGame = false;

	sf::Font myFont;
	sf::Text myClickToContinue;
	sf::Text myStoryText;

	sf::Clock myClock;
	sf::Time myDelay;
	bool myWriteSlow = false;

	std::string myStory;
	unsigned int myCurrentLocation = 0;

	sf::SoundBuffer myBeepBuffers[10];
	sf::Sound myBeepSounds[10];

	State* myParent = nullptr;
};
