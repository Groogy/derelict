#include "topbar.h"
#include "renderer.hpp"
#include "earth.hpp"
#include "label.hpp"
#include "button.hpp"
#include "macros.hpp"

#include <SFML/Window/Event.hpp>

#include <iostream>

void Topbar::setup(Earth& earth, sf::Vector2i targetSize, const Camera2D& camera)
{
	auto& homeostasis = earth.getHomeostasis();
	auto label = std::make_unique<Label>();
	label->attachTextChangeHook([&homeostasis](){
		return "Earth Homeostasis: " + macros::displayFloat(macros::round(homeostasis.getValue(), 100));
	});
	label->move(36, 8);
	myObjects.push_back(std::move(label));

	label = std::make_unique<Label>();
	label->attachTextChangeHook([&homeostasis, &earth](){
		return macros::displayFloatWithSign(macros::round(homeostasis.calculateChange(earth) * 60, 100)) + "/minute";
	});
	label->move(236, 8);
	myObjects.push_back(std::move(label));

	auto& energy = earth.getEnergy();
	label = std::make_unique<Label>();
	label->attachTextChangeHook([&energy, &earth]() {
		return "Available Energy: "  + macros::displayFloat(macros::round(energy.getValue(), 100));
	});
	label->move(36, 40);
	myObjects.push_back(std::move(label));

	label = std::make_unique<Label>();
	label->attachTextChangeHook([&energy, &earth]() {
		return macros::displayFloatWithSign(macros::round(energy.calculateChange(earth) * 60, 100)) + "/minute";
	});
	label->move(236, 40);
	myObjects.push_back(std::move(label));

	auto texture = std::make_unique<sf::Texture>();
	texture->loadFromFile("powerplant.png");
	auto button = std::make_unique<Button>(*texture, "Powerplant");
	button->attachOnClickHook([](){
		std::cout << "HELLO I WAS CLICKED!" << std::endl;
	});
	button->move(10, 100);
	myObjects.push_back(std::move(button));
	myTextures.push_back(std::move(texture));

	texture = std::make_unique<sf::Texture>();
	texture->loadFromFile("homeostasis.png");
	auto sprite = std::make_unique<sf::Sprite>(*texture);
	myIcons.push_back(std::move(sprite));

	label = std::make_unique<Label>();
	label->setString("-0.1/s");
	label->move(48, 164);
	myObjects.push_back(std::move(label));

	sprite = std::make_unique<sf::Sprite>(*texture);
	sprite->move(32, 164);
	sprite->setScale(0.5f, 0.5f);
	myIcons.push_back(std::move(sprite));

	myTextures.push_back(std::move(texture));

	texture = std::make_unique<sf::Texture>();
	texture->loadFromFile("energy.png");
	sprite = std::make_unique<sf::Sprite>(*texture);
	sprite->move(0, 32);
	myIcons.push_back(std::move(sprite));

	label = std::make_unique<Label>();
	label->setString("5");
	label->move(16, 164);
	myObjects.push_back(std::move(label));

	sprite = std::make_unique<sf::Sprite>(*texture);
	sprite->move(0, 164);
	sprite->setScale(0.5f, 0.5f);
	myIcons.push_back(std::move(sprite));

	myTextures.push_back(std::move(texture));

	auto& tilemap = earth.accessTilemap();
	myTilemapClickTest = [&tilemap, targetSize](const sf::Event& event) {
		float offset = targetSize.y / 4;
		sf::IntRect rect(0, offset, targetSize.x, targetSize.y-offset);
		return rect.contains(event.mouseButton.x, event.mouseButton.y);
	};

	label = std::make_unique<Label>();
	label->attachTextChangeHook([this](){
		return myTileInfoStr;
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
	for(auto& obj : myObjects)
	{
		renderer.draw(*obj);
	}
	for(auto& icon : myIcons)
	{
		renderer.draw(*icon);
	}
}

void Topbar::handleOnTilemapClick(const sf::Event& event)
{
	myTilemapClickInspect(event);
}