#include "topbar.h"
#include "renderer.hpp"
#include "earth.hpp"
#include "label.hpp"
#include "button.hpp"
#include "macros.hpp"

#include <SFML/Window/Event.hpp>

#include <iostream>

void Topbar::setupBuildings()
{
	// Power Plant
	auto building = std::make_unique<Building>("Power Plant", "powerplant.png", 5, -0.01, 0.2, false);
	myBuildingsDatabase.push_back(std::move(building));

	// Atmospheric Plant
	building = std::make_unique<Building>("Atmo Plant", "atmoplant.png", 10, 0.005, -0.1, false, [](Earth& earth, const Tile& tile){
		if(!earth.getTilemap().canHaveCloud(tile.getPos()))
			return;

		int chance = rand() % 50;
		if(chance == 0)
		{
			int volume = 8 + rand() % 100;
			earth.spawnCloud(tile.getPos(), volume, false);
		}
	});
	myBuildingsDatabase.push_back(std::move(building));

	// Water Plant
	building = std::make_unique<Building>("Water Plant", "waterplant.png", 15, -0.03, 0.8, false);
	myBuildingsDatabase.push_back(std::move(building));

	// Scrubber
	building = std::make_unique<Building>("Scrubber", "waterplant.png", 50, 0.05, -0.8, false, [](Earth& earth, const Tile& tile){
		int chance = rand() % 5;
		if(chance == 0)
		{
			auto& tilemap = earth.accessTilemap();
			auto& toConvert = tilemap.findFirstConvertable(tile.getPos(), "Barren");
			tilemap.setTileTerrain(toConvert.getPos(), "Barren");
		}
	});
	myBuildingsDatabase.push_back(std::move(building));

	//Nuke
	building = std::make_unique<Building>("Nuke", "waterplant.png", 100, 0, 0, false, [](Earth& earth, const Tile& tile){
		auto& tilemap = earth.accessTilemap();
		auto pos = tile.getPos();
		tilemap.applyFunctionInSphere(pos, 4, [&tilemap, pos](const Tile& tile)
		{
			auto terrainName = tile.getTerrain()->getName();
			if(terrainName == "Water")
			{
				tilemap.setTileTerrain(tile.getPos(), "Ocean Bed");
				return;
			}
			if(terrainName == "Ocean Bed" || terrainName == "Glass" || terrainName == "Mountains")
			{
				return;
			}
			
			auto current = tile.getPos();
			int manhattan = std::abs(current.x - pos.x) + std::abs(current.y - pos.y);
			tilemap.setTileTerrain(current, manhattan <= 1 ? "Glass" : "Wasteland");
			tilemap.setBuilding(current, nullptr);
			
		});
	});
	myBuildingsDatabase.push_back(std::move(building));

	// Polluter
	building = std::make_unique<Building>("Polluter", "waterplant.png", 0, -0.025, 0.0, true, [](Earth& earth, const Tile& tile){
		if(!earth.getTilemap().canHaveCloud(tile.getPos()))
			return;

		int chance = rand() % 500;
		if(chance == 0)
		{
			int volume = 8 + rand() % 50;
			earth.spawnCloud(tile.getPos(), volume, true);
		}
	});
	myBuildingsDatabase.push_back(std::move(building));

	// Comet - Easteregg
	building = std::make_unique<Building>("Comet", "waterplant.png", 0, 0, 0, true, [](Earth& earth, const Tile& tile){
		auto& tilemap = earth.accessTilemap();
		auto pos = tile.getPos();
		tilemap.applyFunctionInSphere(pos, 7, [&tilemap, pos](const Tile& tile)
		{
			auto current = tile.getPos();
			int manhattan = std::abs(current.x - pos.x) + std::abs(current.y - pos.y);
			if(manhattan >= 5 )
				tilemap.setTileTerrain(current, "Mountains");
			else
				tilemap.setTileTerrain(current, manhattan <= 1 ? "Glass" : "Wasteland");
			tilemap.setBuilding(current, nullptr);
		});
	});
	myBuildingsDatabase.push_back(std::move(building));
}

void Topbar::setupGui(Earth& earth, sf::Vector2i targetSize, const Camera2D& camera)
{
	auto texture = std::make_unique<sf::Texture>();
	texture->loadFromFile("topbar.png");
	auto sprite = std::make_unique<sf::Sprite>(*texture);
	myBackground = std::move(sprite);
	myTextures.push_back(std::move(texture));

	auto& homeostasis = earth.getHomeostasis();
	// Homeostasis Label
	auto label = std::make_unique<Label>();
	label->attachTextChangeHook([&homeostasis](){
		return "Homeostasis: " + macros::displayFloat(macros::round(homeostasis.getValue(), 100));
	});
	label->move(38, 8);
	myObjects.push_back(std::move(label));

	// Homeostasis Change label
	label = std::make_unique<Label>();
	label->attachTextChangeHook([&homeostasis, &earth](){
		return macros::displayFloatWithSign(macros::round(homeostasis.calculateChange(earth) , 100));
	});
	label->move(216, 8);
	myObjects.push_back(std::move(label));

	auto& energy = earth.getEnergy();
	// Energy Label
	label = std::make_unique<Label>();
	label->attachTextChangeHook([&energy, &earth]() {
		return "Energy: "  + macros::displayFloat(macros::round(energy.getValue(), 100));
	});
	label->move(38, 40);
	myObjects.push_back(std::move(label));

	// Energy change label
	label = std::make_unique<Label>();
	label->attachTextChangeHook([&energy, &earth]() {
		return macros::displayFloatWithSign(macros::round(energy.calculateChange(earth), 100));
	});
	label->move(216, 40);
	myObjects.push_back(std::move(label));

	texture = std::make_unique<sf::Texture>();
	texture->loadFromFile("homeostasis.png");
	// Large Homeostasis Icon
	sprite = std::make_unique<sf::Sprite>(*texture);
	sprite->move(4, 0);
	myIcons.push_back(std::move(sprite));
	myTextures.push_back(std::move(texture));

	texture = std::make_unique<sf::Texture>();
	texture->loadFromFile("energy.png");
	// Large Energy Icon
	sprite = std::make_unique<sf::Sprite>(*texture);
	sprite->move(4, 32);
	myIcons.push_back(std::move(sprite));

	// Building Info Energy Cost Icon
	sprite = std::make_unique<sf::Sprite>(*texture);
	sprite->move(416, 96);
	sprite->setScale(0.5f, 0.5f);
	myIcons.push_back(std::move(sprite));
	myTextures.push_back(std::move(texture));

	// Building Info Homeostasis Change Icon
	texture = std::make_unique<sf::Texture>();
	texture->loadFromFile("homeostasis_decay.png");
	sprite = std::make_unique<sf::Sprite>(*texture);
	sprite->move(416, 112);
	sprite->setScale(0.5f, 0.5f);
	myIcons.push_back(std::move(sprite));
	myTextures.push_back(std::move(texture));

	// Building Info Energy Change Icon
	texture = std::make_unique<sf::Texture>();
	texture->loadFromFile("energy_decay.png");
	sprite = std::make_unique<sf::Sprite>(*texture);
	sprite->move(416, 130);
	sprite->setScale(0.5f, 0.5f);
	myIcons.push_back(std::move(sprite));
	myTextures.push_back(std::move(texture));

	generateBuildingButtons(earth, targetSize, camera);

	// Tile Inspection info

	auto& tilemap = earth.accessTilemap();
	myTilemapClickTest = [&tilemap, targetSize](const sf::Event& event) {
		float offset = targetSize.y / 4;
		sf::IntRect rect(0, offset, targetSize.x, targetSize.y-offset);
		return rect.contains(event.mouseButton.x, event.mouseButton.y);
	};

	label = std::make_unique<Label>();
	label->attachTextChangeHook([this]() {
		std::string result = "SELECTED TILE\n";
		result += myTileInfoStr;
		return result;
	});
	label->move(325, 8);
	myObjects.push_back(std::move(label));

	myTilemapClickInspect = [&tilemap, targetSize, &camera, this](const sf::Event& event) {
		float offset = targetSize.y / 4;
		sf::Vector2i clickOnScreen(event.mouseButton.x, event.mouseButton.y - offset);
		auto tile = tilemap.findTileFromClick(clickOnScreen, camera);
		myTileInfoStr = tile.getInfoStr();
		tilemap.selectTile(tile.getPos());
	};

	// Building info

	label = std::make_unique<Label>();
	label->attachTextChangeHook([this]() {
		std::string result = "BUILDING STATS\n";
		result += myBuildingStatsStr;
		return result;
	});
	label->move(435, 64);
	myObjects.push_back(std::move(label));
}

void Topbar::update()
{
	for(auto& obj : myObjects)
	{
		obj->update();
	}
}

void Topbar::processEvent(const sf::Event& event)
{
	for(auto& obj : myObjects)
	{
		obj->processEvent(event);
	}

	switch(event.type)
	{
		case sf::Event::MouseButtonReleased:
		{
			if(myTilemapClickTest(event))
			{
				handleOnTilemapClick(event);
			}
			break;
		}
		default:
			break;
	}
}

void Topbar::render(Renderer& renderer) const
{
	renderer.draw(*myBackground);
	for(auto& obj : myObjects)
	{
		renderer.draw(*obj);
	}
	for(auto& icon : myIcons)
	{
		renderer.draw(*icon);
	}
}

const std::vector<std::unique_ptr<Building>>& Topbar::getBuildings() const
{
	return myBuildingsDatabase;
}

void Topbar::handleOnTilemapClick(const sf::Event& event)
{
	if(myTilemapClickAction)
	{
		myTilemapClickAction(event);
		myTilemapClickAction = nullptr;
	}
	else
	{
		myTilemapClickInspect(event);
	}
}

void Topbar::generateBuildingButtons(Earth& earth, sf::Vector2i targetSize, const Camera2D& camera)
{
	sf::Vector2f pos(5, 64);
	for(auto& building : myBuildingsDatabase)
	{
		if(building->isHidden())
			continue;
		generateBuildingButtonFor(*building, earth, targetSize, camera, pos);
		pos.x += 70;
		if ( pos.x > 230)
		{
			pos.x = 5;
			pos.y += 64;
		}
	}
}

void Topbar::generateBuildingButtonFor(const Building& building, Earth& earth, sf::Vector2i targetSize, const Camera2D& camera, sf::Vector2f pos)
{
	auto texture = std::make_unique<sf::Texture>();
	texture->loadFromFile(building.getButton());
	auto button = std::make_unique<Button>(*texture, building.getName());
	button->attachOnClickHook([this, targetSize, &building, &earth, &camera](){
		if(!earth.canBuild(building))
		{
			// TODO Failure sound
			return;
		}
		earth.accessTilemap().resetSelected();
		myTilemapClickAction = [&earth, &camera, &building, targetSize, this](const sf::Event& event) {
			float offset = targetSize.y / 4;
			sf::Vector2i clickOnScreen(event.mouseButton.x, event.mouseButton.y - offset);
			auto tile = earth.getTilemap().findTileFromClick(clickOnScreen, camera);
			if(earth.canBuildOn(tile.getPos(), building))
				earth.buildOn(tile.getPos(), building);
			
			// TODO Failure sound
		};
	});
	button->attachOnHoverHook([this, &building](){
		myBuildingStatsStr = building.getStatsStr();
	});
	button->move(pos);
	myObjects.push_back(std::move(button));
	myTextures.push_back(std::move(texture));
}