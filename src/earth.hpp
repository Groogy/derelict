#pragma once

#include "renderable.hpp"
#include "tilemap.hpp"
#include "homeostasis.hpp"
#include "energy.hpp"

class Earth : public Renderable
{
public:
	Earth();
	virtual ~Earth();

	const Tilemap& getTilemap() const;
	Tilemap& accessTilemap();

	const Homeostasis& getHomeostasis() const;
	Homeostasis& accessHomeostasis();

	const Energy& getEnergy() const;
	Energy& accessEnergy();

	void update();

private:
	void generateSphere(float radius);

	Tilemap myTilemap;
	Homeostasis myHomeostasis;
	Energy myEnergy;
};
