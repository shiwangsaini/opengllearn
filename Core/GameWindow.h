#pragma once

#include <glad/gl.h>
#include <iostream>

#include "InputHandler.h" 
#include "VertexBuffer.h"
#include "Texture.h"

// Handles game Window
class GameWindow
{
public:
	GameWindow(int width, int height, const char* title) : m_width(width), m_height(height), m_title(title) {}
	~GameWindow() {};

	bool init();
	void run();
	void shutdown();

private:
	int m_width;
	int m_height;
	InputHandler m_inputHandler = NULL;
	const char* m_title = nullptr;
	GLFWwindow* m_window = nullptr;

	Shader shader;
	VertexBuffer VertexBuffer;
	Texture texture;
};