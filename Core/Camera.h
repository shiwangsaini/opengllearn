#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shaders/shaders.h"

class Camera
{
public:
	Camera() {}
	Camera(GLFWwindow* window, Shader* shader);
	~Camera() {};

	void update();
	void setFov(float& fov);

	glm::vec3 mPos;
	glm::vec3 mFront;
	glm::vec3 mUp;

private:
	Shader* mShader;

	glm::mat4 mProjection;
	glm::mat4 mView;

	GLFWwindow* mWindow;
	int* mWidth;
	int* mHeight;
	float mFov = 70.0f;
};

