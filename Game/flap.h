#pragma once

#include <vector>
#include <glm.hpp>
#include <random>
#include "InputHandler.h"

struct Obstacles {
	int top;
	int bottom;
	int nextDistance;
};

class FlapBird {
	// jump
	// end

public:
	FlapBird(InputHandler* inHandle);
	~FlapBird() {}
	
	Obstacles MakeObstacle();
	void FillObstacles(std::vector<glm::vec3>& data);
	void DestroyObstacles();

	void jump(bool& isJumping, bool& againJump, float& dt);
	void fall(bool& isFalling, float& dt);

	void Render(float& dt);

	const glm::vec3 GetBird() { return m_bird; }

private:
	std::mt19937 m_gen{ std::random_device{}() };
	int m_obstecleMaxDistance;
	int m_obstecleMinDistance;
	int m_obstecleMaxHeight;
	int m_obstecleMinHeight;

	glm::vec3 m_camPos;
	glm::vec3 m_bird;
	std::vector<glm::vec3> m_map;

	InputHandler* m_inputs;

	float m_moveSpeed = 2.0;

};