#include "guiobject.hpp"

GuiObject::~GuiObject()
{

}

void GuiObject::update()
{
	for(auto& hook : myUpdateHooks)
	{
		hook(*this);
	}
}

void GuiObject::processEvent(const sf::Event&)
{
}

void GuiObject::attachUpdateHook(UpdateHook hook)
{
	myUpdateHooks.push_back(hook);
}
