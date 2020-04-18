#include "earth.hpp"
#include "macros.hpp"

#include <iostream>

#include <cmath>

Earth::Earth()
: myTilemap("earth.png")
{
	generateSphere(10.0);
}

Earth::~Earth()
{
	
}

const Tilemap& Earth::getTilemap() const
{
	return myTilemap;
}

Tilemap& Earth::accessTilemap()
{
	return myTilemap;
}

void Earth::generateSphere(float radius)
{
	constexpr float Pi = 3.141592654f;
	constexpr int SectorCount = 256;
	constexpr int StackCount = 256;
	constexpr float SectorStep = 2 * Pi / SectorCount;
	constexpr float StackStep = Pi / StackCount;

	const float radiusInv = 1.0 / radius;

	std::vector<Vertex> tmpVertices;
	
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
			tmpVertices.emplace_back(
				sf::Vector3f(x, y, z), 
				sf::Vector2f(static_cast<float>(sector + 1) / SectorCount, static_cast<float>(stack + 1) / StackCount), 
				sf::Vector3f(x * radiusInv, y * radiusInv, z * radiusInv)
			);
		}
	}

	int index = 0;
	for(int stack = 0; stack < StackCount; stack++)
	{
		int k1 = stack * (SectorCount + 1);
		int k2 = (stack + 1) * (SectorCount + 1);
		for(int sector = 0; sector < SectorCount; sector++, k1++, k2++)
		{
			auto v1 = tmpVertices[k1];
			auto v2 = tmpVertices[k2];
			auto v3 = tmpVertices[k1 + 1];
			auto v4 = tmpVertices[k2 + 1];

			if(stack == 0)
			{
				myVertices.push_back(v2);
				myVertices.push_back(v1);
				myVertices.push_back(v4);

				myIndices.push_back(index++);
				myIndices.push_back(index++);
				myIndices.push_back(index++);
			}
			else if(stack == StackCount - 1)
			{
				myVertices.push_back(v1);
				myVertices.push_back(v2);
				myVertices.push_back(v3);

				myIndices.push_back(index++);
				myIndices.push_back(index++);
				myIndices.push_back(index++);
			}
			else
			{
				myVertices.push_back(v1);
				myVertices.push_back(v2);
				myVertices.push_back(v3);
				myVertices.push_back(v4);

				myIndices.push_back(index);
				myIndices.push_back(index+1);
				myIndices.push_back(index+2);

				myIndices.push_back(index+2);
				myIndices.push_back(index+1);
				myIndices.push_back(index+3);
				
				index += 4;
			}
			
		}
	}
}