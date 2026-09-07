#pragma once

#include "Input.h"
#include <chrono>

#include "Camera.h"

// handle input
class InputHandler
{
public:
	InputHandler() {};
	InputHandler(GLFWwindow* window, Shader* shader);
	~InputHandler() {};

	//void handleInput(float& deltaTime);
	void keyHandle(float& dt);
	void mouseHandle(float& deltaTime, float& x, float& y, float& z, const unsigned int& width, const unsigned int& height);
	void cameraHandle(float& deltaTime);
	void scrollHandle();
	void updateMVP(glm::vec3& Position);

private:
	GLFWwindow* m_window = nullptr;
	bool cursorEnabled = true;

	bool escWasPressed = false;

	float yaw = -90.0f;
	float pitch = 0.0f;

	float lastX = 640.0f;
	float lastY = 460.0f;

	float lastcameraPos;

	bool firstMouse = true;

	Camera mCamera;
	Shader* mShader;
};

// command class to handle actions of input
class command 
{
public:

private:

};

