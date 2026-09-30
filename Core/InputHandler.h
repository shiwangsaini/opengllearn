#pragma once

#include "Input.h"
#include <chrono>

#include "Camera.h"

// handle input
class InputHandler
{
public:
	InputHandler() = default;
	InputHandler(GLFWwindow* window, std::vector<Shader>& shaders);
	~InputHandler() {};

	//void handleInput(float& deltaTime);
	void keyHandle(float& dt);
	void mouseHandle(float& deltaTime, float& x, float& y, float& z, const unsigned int& width, const unsigned int& height);
	void cameraHandle(float& deltaTime);
	void scrollHandle();
	void updateMVP(glm::vec3& Position, const ShaderType& Type);
	void updateMVPScale(glm::vec3& Position, glm::vec3& scales, Shader& shader);
	void updateMVPRotation(glm::vec3& Position, float& radian, float& rotateX , float& rotateY, Shader& shader);

	void useShader(const ShaderType& Type);

	const glm::vec3 getCameraPos() const { return mCamera.mPos; }
	void SetCameraPos(glm::vec3& camPos);

	std::vector<Shader>& mShaders;
	Camera mCamera;

private:
	GLFWwindow* m_window = nullptr;
	bool cursorEnabled = true;

	bool escWasPressed = false;

	float yaw = 90.0f;
	float pitch = 0.0f;

	float lastX = 640.0f;
	float lastY = 460.0f;

	float lastcameraPos = 0.0f;

	bool firstMouse = true;

};

// command class to handle actions of input
class command 
{
public:

private:

};

