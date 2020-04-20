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

class IntroState : public State
{
public:
	IntroState(sf::RenderWindow& window);

	virtual State* update() override;

private:
	void updateText();

	sf::RenderWindow& myWindow;
	bool myStartGame = false;

	sf::Font myFont;
	sf::Text myClickToContinue;
	sf::Text myStoryText;

	sf::Clock myClock;
	sf::Time myDelay;

	std::stringstream myStoryBuffer;
	std::stringstream myInstructionBuffer;
	unsigned int myCurrentLocation = 0;

	sf::SoundBuffer myBeepBuffers[10];
	sf::Sound myBeepSounds[10];
};
