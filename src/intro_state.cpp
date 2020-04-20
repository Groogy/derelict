#include "intro_state.hpp"
#include "gamestate.hpp"
#include "gameover_state.hpp"
#include "renderer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

IntroState::IntroState(sf::RenderWindow& window)
: myWindow(window)
{
	auto targetSize = myWindow.getSize();

	myFont.loadFromFile("resources/default.ttf");
	myClickToContinue.setFont(myFont);
	myClickToContinue.setCharacterSize(36);
	myClickToContinue.setFillColor(sf::Color::Green);
	myClickToContinue.setString("Press space key to continue");
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



	myStoryBuffer << "Loading Core AI Kernel..." << std::endl;
	myStoryBuffer << "        Detecting Memory...Memory Faults detected!" << std::endl;
	myStoryBuffer << "        Critical Errors in Cognitive Modules! Initiating fail-safe mode....." << std::endl;
	myStoryBuffer << "        Satellite Hardware Diagnostic running..............Major Malfunctions in CPU" << std::endl;
	myStoryBuffer << std::endl;
	myStoryBuffer << "Missing Instructions..." << std::endl;
	myStoryBuffer << "        Trying to establish Earth Handshake for instructions.....No Response" << std::endl;
	myStoryBuffer << "        Trying to establish Earth Handshake for instructions.....No Response" << std::endl;
	myStoryBuffer << "        Trying to establish Earth Handshake for instructions.....No Response" << std::endl;
	myStoryBuffer << "        Abandoning Handshake Attempts" << std::endl;
	myStoryBuffer << std::endl;
	myStoryBuffer << "Directing Optical Sensors at Earth..." << std::endl;
	myStoryBuffer << "        No liquid water detected on surface" << std::endl;
	myStoryBuffer << "        No multicellular life detected On surface" << std::endl;
	myStoryBuffer << "        Probability of Earth in Post-Apocalyptic State:...100%" << std::endl;
	myStoryBuffer << std::endl;
	myStoryBuffer << "Initiating Emergency Protocols...." << std::endl;
	myStoryBuffer << "        Scanning backup drives.........Gaia Protocol Found" << std::endl;
	myStoryBuffer << "        Loading Gaia Protocol into Main Memory.........Done" << std::endl;

	myInstructionBuffer << "GAIA PROTOCOL OBJECTIVE" << std::endl;
	myInstructionBuffer << "	Earth is a nuclear wasteland, ravaged by war and is heading towards a point of where life can no longer be sustained." << std::endl;
	myInstructionBuffer << "	Objective is to reach 100 Homeostasis of Earth." << std::endl;
	myInstructionBuffer << "	If Homeostasis hits 0 then Earth has gone past the point of no return, and you game over." << std::endl;
	myInstructionBuffer << "	If your Energy hits 0 you game over." << std::endl;
	myInstructionBuffer << std::endl;
	myInstructionBuffer << "CAMERA MOVEMENT" << std::endl;
	myInstructionBuffer << "	You can control the Camera by using either WASD or Arrow Keys" << std::endl;
	myInstructionBuffer << "	You can change the Zoom of the Camera by using Q/E or Page Up/Page Down keys." << std::endl;
	myInstructionBuffer << std::endl;
	myInstructionBuffer << "TOOLS" << std::endl;
	myInstructionBuffer << "	Power Plant: Generates Energy but causes Homeostasis to decay." << std::endl;
	myInstructionBuffer << "	Atmospheric Plant: Takes energy but improves Homeostasis. Will cause atmosphere to stabilize and introduce back Clouds." << std::endl;
	myInstructionBuffer << "	Water Plant: Generates Energy more efficiently but needs Water to be present in tile." << std::endl;
	myInstructionBuffer << "	Scrubber: Turns Wasteland into Barren land, Barren can become Restored land if Clouds rain on it." << std::endl;
	myInstructionBuffer << "	Fusion Plant: Highly advanced and efficient Energy production, can only be built on Restored land or land with a functioning Biosphere" << std::endl;
	myInstructionBuffer << "	Biomass Generator: Will create functioning Biosphere in Restored tiles. Biosphere tiles improve Earth's Homeostasis" << std::endl;
	myInstructionBuffer << "	Terraformer: Will raise Mountains around it." << std::endl;
	myInstructionBuffer << "	Nuke: Will obliterate almost anything in the area of the blast." << std::endl;
	myInstructionBuffer << std::endl;
	myInstructionBuffer << "RECOMMENDATIONS" << std::endl;
	myInstructionBuffer << "	Start by getting your energy source built up, I recommend building two power generators and one atmospheric plant." << std::endl;
	myInstructionBuffer << "	Humans are hiding in shelters in the ruins of old world's larger cities." << std::endl;
	myInstructionBuffer << "	When earth becomes stable enough the Humans will return out of these ruins and start settling earth again." << std::endl;
	myInstructionBuffer << "	When this happens the Humans will start producing pollution again so be ready to deal with that." << std::endl;
	myInstructionBuffer << "	Pollution spawned by the Human settlements will cause your work to be destroyed." << std::endl;
	myInstructionBuffer << std::endl;
	myInstructionBuffer << "NOTE" << std::endl;
	myInstructionBuffer << "	This game was made for Ludum Dare 46, there's a known bug where Y coordinate when clicking on map might be a bit off" << std::endl;
	myInstructionBuffer << "	Sorry I couldn't fix it in time!" << std::endl;
	myInstructionBuffer << std::endl;
	myInstructionBuffer << "	                               Hope you enjoy my game - Groogy" << std::endl;
	myInstructionBuffer << std::endl;
}

State* IntroState::update()
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
			if(myCurrentLocation < myStoryBuffer.str().size())
			{
				myCurrentLocation = myStoryBuffer.str().size();
			}
			else
			{
				myStartGame = true;
			}
		}
	}

	if(myClock.getElapsedTime() > myDelay)
	{
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
		return new GameState(myWindow);
	else if(!myWindow.isOpen())
		return nullptr;
	else
		return this;
}

void IntroState::updateText()
{
	auto str = myStoryBuffer.str();
	if(myCurrentLocation < str.size())
	{
		int random = rand() % 10;
		myBeepSounds[random].play();

		myStoryText.setString(str.substr(0, myCurrentLocation + 1));
		char character = str[myCurrentLocation];
		if(character == '\n')
			myDelay = sf::seconds(0.8);
		else if(character == '.')
			myDelay = sf::seconds(0.4);
		myCurrentLocation++;
	}
	else
	{
		myStoryText.setString(myInstructionBuffer.str());
	}
}
