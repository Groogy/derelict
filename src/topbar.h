#pragma once

#include "guiobject.hpp"
#include "building.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <memory>

class Earth;
class Renderer;
class Camera2D;

class Topbar
{
public:
	void setupSound();
	void setupBuildings();
	void setupGui(Earth& earth, sf::Vector2i targetSize, const Camera2D& camera);

	void update();
	void processEvent(const sf::Event& event);
	void render(Renderer& renderer) const;

	sf::Time getTimeStep() const;

	const std::vector<std::unique_ptr<Building>>& getBuildings() const;

private:
	void handleOnTilemapClick(const sf::Event& event);

	void generateBuildingButtons(Earth& earth, sf::Vector2i targetSize, const Camera2D& camera);
	void generateBuildingButtonFor(const Building& building, Earth& earth, sf::Vector2i targetSize, const Camera2D& camera, sf::Vector2f pos);

	std::function<bool(const sf::Event&)> myTilemapClickTest;
	std::function<void(const sf::Event&)> myTilemapClickInspect;
	std::function<void(const sf::Event&)> myTilemapClickAction;

	std::unique_ptr<sf::Sprite> myBackground;
	std::vector<std::unique_ptr<GuiObject>> myObjects;
	std::vector<std::unique_ptr<sf::Sprite>> myIcons;
	std::vector<std::unique_ptr<sf::Texture>> myTextures;

	std::string myTileInfoStr;
	std::string myBuildingStatsStr;

	std::vector<std::unique_ptr<Building>> myBuildingsDatabase;

	sf::SoundBuffer myFailureSoundBuffer;
	sf::SoundBuffer mySuccessSoundBuffer;
	sf::Sound myFailureSound;
	sf::Sound mySuccessSound;

	sf::SoundBuffer myNukeRadiationSoundBuffer;
	sf::SoundBuffer myNukeExplosionSoundBuffer;
	sf::Sound myNukeRadiationSound;
	sf::Sound myNukeExplosionSound;

	sf::SoundBuffer myEarthquakeSoundBuffer;
	sf::Sound myEarthquakeSound;

	enum Timestep 
	{
		Normal,
		Fast,
		Faster
	} myTimestep;
};
