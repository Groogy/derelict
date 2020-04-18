#pragma once

#include "vertex.hpp"
#include "transform.hpp"

#include <vector>

class Renderable
{
public:
	virtual ~Renderable();

	Transform getTransform() const;
	void setTransform(Transform transform);

	const std::vector<Vertex>& getVertices() const;
	const std::vector<IndexType>& getIndices() const;

protected:
	std::vector<Vertex> myVertices;
	std::vector<IndexType> myIndices;	
	Transform myTransform = Transform::Identity;
};
