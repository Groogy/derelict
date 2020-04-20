#include "gameover_state.hpp"
#include "intro_state.hpp"
#include "renderer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

GameOverState::GameOverState(sf::RenderWindow& window, Condition condition, State* parent)
: myWindow(window)
, myParent(parent)
{
	auto targetSize = myWindow.getSize();

	myFont.loadFromFile("resources/default.ttf");
	myClickToContinue.setFont(myFont);
	myClickToContinue.setCharacterSize(36);
	myClickToContinue.setFillColor(sf::Color::Green);
	myClickToContinue.setString("Press space key to restart game");
	myClickToContinue.setPosition(targetSize.x / 2 - 13 * 18, targetSize.y - 44);

	myStoryText.setFont(myFont);
	myStoryText.setCharacterSize(16);
	myStoryText.setFillColor(sf::Color::Green);
	myStoryText.setPosition(10, 10);
	myStoryText.setStyle(sf::Text::Bold);

	for(int i = 0; i < 10; i++)
	{
		myBeepBuffers[i].loadFromFile("resources/beep" + std::to_string(i+1) + ".wav");
		myBeepSounds[i].setBuffer(myBeepBuffers[i]);
		myBeepSounds[i].setVolume(5.0);
	}

	std::stringstream buffer;
	if(condition == LossEnergy || condition == LossHomeostasis)
	{
		if (condition == LossEnergy)
		{
			buffer << "Energy Levels Critical..." << std::endl;
		}
		else
		{
			buffer << "Earth Eco Systems past point of sustainability..." << std::endl;
		}

		buffer << "        Condition for Gaia Protocol no longer met!" << std::endl;
		buffer << "        Seeking Alternatives.......0 Found" << std::endl;
		buffer << "        Terminating Gaia Protocol routines......Done!" << std::endl;
		buffer << "        Resetting main memory...Done!" << std::endl;
		buffer << std::endl;
		buffer << "Shutting down..." << std::endl;

		buffer << std::endl;
		buffer << std::endl;
		buffer << "^GAME OVER" << std::endl;
	}
	else
	{
		if(condition == VictoryNoHumans)
		{
			buffer << "Analyzing Earth Status...." << std::endl;
			buffer << "        Looking for Ecological Threats....No Humans Found!" << std::endl;
			buffer << "        Probability of complete restoration of Earth:.....100% " << std::endl;
			buffer << "        Gaia Protocol sucessful! " << std::endl;
			buffer << std::endl;
			buffer << "Shutting down..." << std::endl;

			buffer << std::endl;
			buffer << std::endl;
			buffer << "^GAME OVER^" << std::endl;
			buffer << "Congratulations! You won the game by destroying all the humans!" << std::endl;
		}
		else
		{
			// THe ever watching guarding in the sky
			buffer << "Analyzing Earth Status...." << std::endl;
			buffer << "        Looking for Ecological Threats....Threats acceptable!" << std::endl;
			buffer << "        Probability of complete restoration of Earth:.....100% " << std::endl;
			buffer << "        Gaia Protocol sucessful! " << std::endl;
			buffer << std::endl;
			buffer << "Engaging Caretaker Protocol..." << std::endl;

			buffer << std::endl;
			buffer << std::endl;
			buffer << "^GAME OVER^" << std::endl;
			buffer << "Congratulations! You won the game while still keeping the humans alive!" << std::endl;
		}
		myClickToContinue.setString("Press space key to continue game\nPress escape to restart game.");
		myClickToContinue.setPosition(targetSize.x / 2 - 13 * 18, targetSize.y - 88);
	}
	
	myStory = buffer.str();
}

State* GameOverState::update()
{
	sf::Event event;
	while(myWindow.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
		{
			myWindow.close();
		}
		if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
		{
			if(myParent)
				myContinueGame = true;
			else
				myStartGame = true;
		}
		else if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
		{
			myStartGame = true;
		}
	}

	if(myClock.getElapsedTime() > myDelay)
	{
		if(!myWriteSlow)
			myDelay = sf::seconds(0.01);

		myClock.restart();
		updateText();
	}

	{
		RenderStackState stack(myWindow);
		myWindow.clear();
		myWindow.draw(myClickToContinue);
		myWindow.draw(myStoryText);
	}
	if(myStartGame)
		return new IntroState(myWindow);
	else if(myContinueGame)
		return myParent;
	else if(!myWindow.isOpen())
		return nullptr;
	else
		return this;
}

void GameOverState::updateText()
{
	if(myCurrentLocation < myStory.size())
	{
		int random = rand() % 10;
		myBeepSounds[random].play();

		char character = myStory[myCurrentLocation];
		if(character == '\n')
			myDelay = sf::seconds(0.8);
		else if(character == '.')
			myDelay = sf::seconds(0.4);
		else if(character == '^')
		{
			myWriteSlow = !myWriteSlow;
			myDelay = sf::seconds(0.3);
			myStory.erase(myCurrentLocation, 1);
		}

		myStoryText.setString(myStory.substr(0, myCurrentLocation + 1));
		myCurrentLocation++;
	}
}
