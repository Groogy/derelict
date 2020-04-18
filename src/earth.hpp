#pragma once

#include "renderable.hpp"

class Earth : public Renderable
{
public:
	Earth();
	virtual ~Earth();

private:
	void generateSphere(float radius);
};
