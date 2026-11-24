#pragma once

#include <glm.hpp>

namespace Model
{
	struct BoundingBox {
		glm::vec3 minBounds{ 0.f };
		glm::vec3 maxBounds{ 0.f };

		glm::vec3 GetDimension() const { return (maxBounds - minBounds); }
		glm::vec3 GetCenter() const { 
			glm::vec3 temp = maxBounds + minBounds;
			return { temp.x / 2, temp.y / 2, temp.z / 2 };
		}
	};

	struct Material
	{
		glm::vec3 diffuse{ 0.0f };
		glm::vec3 specular{ 0.0f };
		glm::vec3 ambient{ 0.0f };

		float shininess = 0.0f;
		bool useColor = false;
	};
}
