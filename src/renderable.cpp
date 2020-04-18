#include "renderable.hpp"

Renderable::~Renderable()
{
}

Transform Renderable::getTransform() const
{
	return myTransform;
}

void Renderable::setTransform(Transform transform)
{
	myTransform = transform;
}

const std::vector<Vertex>& Renderable::getVertices() const
{
	return myVertices;
}

const std::vector<IndexType>& Renderable::getIndices() const
{
	return myIndices;
}