#pragma once

#include "renderable.hpp"
#include "tilemap.hpp"

class Earth : public Renderable
{
public:
	Earth();
	virtual ~Earth();

	const Tilemap& getTilemap() const;
	Tilemap& accessTilemap();

private:
	void generateSphere(float radius);

	Tilemap myTilemap;
};
