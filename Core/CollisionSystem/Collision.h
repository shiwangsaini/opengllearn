#pragma once

#include "Model/struct.h"

class Collision
{
public:
	Collision() {};
	~Collision() {};
	//shape, size
	static bool Detect(const Model::BoundingBox& BB1, const glm::vec3& Position1, const Model::BoundingBox& BB2, const glm::vec3& Position2);
	// static object
	
	// dynamic obj
	
	//Resolution
};

