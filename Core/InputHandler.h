#pragma once

#include "Input.h"
#include <chrono>
#include <glm.hpp>

// handle input
class InputHandler
{
public:
	InputHandler() {};
	InputHandler(GLFWwindow* window);
	~InputHandler() {};

	//void handleInput(float& deltaTime);
	void keyHandle(float& dt, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp);
	void mouseHandle(float& deltaTime, float& x, float& y, float& z, const unsigned int& width, const unsigned int& height);
	void cameraHandle(glm::vec3& cameraFront);

private:
	GLFWwindow* m_window = nullptr;
	bool cursorEnabled = false;

	bool escWasPressed = false;

	float yaw = -90.0f;
	float pitch = 0.0f;

	float lastX = 640.0f;
	float lastY = 460.0f;
	bool firstMouse = true;
};

// command class to handle actions of input
class command 
{
public:

private:

};

