#pragma once

#include "Input.h"

#include "Camera.h"

namespace KeyBind {
	// Set key Bindings
	void BindKeyPress(int key, std::function<void()>func);
	void BindKeyOnHold(int key, std::function<void()>func);
	void BindKeyRelease(int key, std::function<void()>func);
}

// handle input
class InputHandler
{
public:
	InputHandler() = default;
	InputHandler(GLFWwindow* window, Shader& shaders);
	~InputHandler() {};

	//void handleInput(float& deltaTime);
	void keyHandle(float& dt);
	void mouseHandle(float& deltaTime, float& x, float& y, float& z, const unsigned int& width, const unsigned int& height);
	void cameraHandle(float& deltaTime);
	void scrollHandle();
	void updateMVP(const glm::vec3& Position, const ShaderType& Type);
	void updateMVP(const glm::vec3& Position, const glm::vec3& scale, const ShaderType& type);
	void updateMVP(const glm::vec3& Position, const glm::vec3& scale, const float& radian, const glm::vec3& rotation, const ShaderType& type);

	void useShader(const ShaderType& Type);

	const glm::vec3 getCameraPos() const { return mCamera.mPos; }
	void SetCameraPos(glm::vec3& camPos);
	void CursorOn(const bool& on);

	bool isCursorEnable() { return cursorEnabled; }

	Shader& mShaders;
	Camera mCamera;

private:
	GLFWwindow* m_window = nullptr;
	bool cursorEnabled = true;

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

