#pragma once

#include "CallbackFunctions.h"
#include <chrono>

// handle input
class InputHandler
{
public:
	InputHandler(GLFWwindow* window);
	~InputHandler() {};

	void handleInput(float deltaTime);

private:
	void keyHandle(float deltaTime);
	void mouseHandle(float deltaTime);
};

// command class to handle actions of input
class command 
{
public:

private:

};
