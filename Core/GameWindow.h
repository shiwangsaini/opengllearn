#pragma once

#include <iostream>
#include "Texture.h"
#include "InputHandler.h"

// Handles game Window
class GameWindow
{
public:
	GameWindow(int width, int height, const char* title) : m_width(width), m_height(height), m_title(title) {}
	~GameWindow() {};

	void init();
	void run();
	void shutdown();

private:
	void Draw();

	int m_width;
	int m_height;
	const char* m_title = nullptr;
	GLFWwindow* m_window = nullptr;

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	Shader shader;
	Texture texture;
	InputHandler inHandler = NULL;
};