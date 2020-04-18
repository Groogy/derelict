#pragma once

#include "contracts.hpp"

class Terrain;

class Tile
{
public:
	Tile(const Terrain* terrain);

private:
	const Terrain* myTerrain;

	BEGIN_INVARIANT(Tile)
		INVARIANT(self.myTerrain != nullptr);
	END_INVARIANT()
};
