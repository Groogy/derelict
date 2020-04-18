#include "earth.hpp"

#include <cmath>

Earth::Earth()
{
	generateSphere(1.0);
}

Earth::~Earth()
{
	
}

void Earth::generateSphere(float radius)
{
	constexpr float Pi = 3.141592654f;
	constexpr int SectorCount = 64;
	constexpr int StackCount = 64;
	constexpr float SectorStep = 2 * Pi / SectorCount;
	constexpr float StackStep = Pi / StackCount;

	const float radiusInv = 1.0 / radius;
	
	for(int stack = 0; stack <= StackCount; stack++)
	{
		float stackAngle = Pi / 2 - stack * StackStep;
		float xy = radius * std::cos(stackAngle);
		float z = radius * std::sin(stackAngle);
		for(int sector = 0; sector <= SectorCount; sector++)
		{
			float sectorAngle = sector * SectorStep;
			float x = xy * std::cos(sectorAngle);
			float y = xy * std::sin(sectorAngle);
			myVertices.emplace_back(
				sf::Vector3f(x, y, z), 
				sf::Vector2f(static_cast<float>(sector) / SectorCount, static_cast<float>(stack) / StackCount), 
				sf::Vector3f(x * radiusInv, y * radiusInv, z * radiusInv)
			);
		}
	}

	for(int stack = 0; stack < StackCount; stack++)
	{
		int k1 = stack * (SectorCount + 1);
		int k2 = k1 + SectorCount + 1;
		for(int sector = 0; sector < SectorCount; sector++, k1++, k2++)
		{
			if(stack != 0)
			{
				myIndices.push_back(k1);
				myIndices.push_back(k2);
				myIndices.push_back(k1 + 1);
			}

			if(stack != (StackCount - 1))
			{
				myIndices.push_back(k1 + 1);
				myIndices.push_back(k2);
				myIndices.push_back(k2 + 1);
			}
		}
	}
}