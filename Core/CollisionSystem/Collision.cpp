#include "Collision.h"
#include <iostream>

bool Collision::Detect(const Model::BoundingBox& BB1, const glm::vec3& Position1, const Model::BoundingBox& BB2, const glm::vec3& Position2)
{
	glm::vec3 min1 = (BB1.minBounds + Position1);
	glm::vec3 max1 = (BB1.maxBounds + Position1);
	glm::vec3 min2 = (BB2.minBounds + Position2);
	glm::vec3 max2 = (BB2.maxBounds + Position2);

	if (max1.x < min2.x || min1.x > max2.x) return false;
	if (max1.y < min2.y || min1.y > max2.y) return false;
	if (max1.z < min2.z || min1.z > max2.z) return false;

	return true;
}
