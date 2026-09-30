#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shaders/shaders.h"

class Camera
{
public:
	Camera() {}
	Camera(GLFWwindow* window);
	~Camera() {};

	void update(Shader& shader);
	void setFov(float& fov);

	glm::vec3 mPos	{0.0f};
	glm::vec3 mFront{0.0f};
	glm::vec3 mUp	{0.0f};

private:
	glm::mat4 mProjection;
	glm::mat4 mView;

	GLFWwindow* mWindow;
	int mWidth;
	int mHeight;
	float mFov = 45.0f;
};

