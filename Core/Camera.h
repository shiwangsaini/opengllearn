#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shaders/shaders.h"

class Camera
{
public:
	Camera() = default;
	Camera(GLFWwindow* window);
	~Camera() {};

	void update(Shader& shader, const uint16_t& type);
	void setFov(float& fov);

	glm::vec3 mPos	{0.0f};
	glm::vec3 mFront{0.0f};
	glm::vec3 mUp	{0.0f};

private:
	glm::mat4 mProjection{0};
	glm::mat4 mView{0};

	GLFWwindow* mWindow;
	int mWidth = 0;
	int mHeight = 0;
	float mFov = 45.0f;
};

