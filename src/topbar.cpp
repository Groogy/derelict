#include "topbar.h"
#include "renderer.hpp"
#include "earth.hpp"
#include "label.hpp"
#include "button.hpp"
#include "macros.hpp"

#include <iostream>

void Topbar::setup(Earth& earth)
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
