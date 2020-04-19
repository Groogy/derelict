#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <functional>
#include <vector>

namespace sf
{
	class Event;
}

class GuiObject : public sf::Drawable, public sf::Transformable
{
public:
	using UpdateHook = std::function<void(GuiObject&)>;

	virtual ~GuiObject();

	virtual void update();
	virtual void processEvent(const sf::Event& event);

	void attachUpdateHook(UpdateHook hook);

private:
	std::vector<UpdateHook> myUpdateHooks;
};
