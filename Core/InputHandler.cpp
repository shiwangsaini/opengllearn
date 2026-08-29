#include "InputHandler.h"
#include <iostream>


InputHandler::InputHandler(GLFWwindow* window)
{
	Input::initInput(window);
}

void InputHandler::handleInput(float deltaTime)
{
	//mouseHandle();
	keyHandle(deltaTime);
}

void InputHandler::keyHandle(float deltaTime)
{
	if (Input::isKeyPressed(GLFW_KEY_A)) std::cout << "pressed A\n";
	if (Input::isKeyPressed(GLFW_KEY_W)) std::cout << "pressed W\n";
	if (Input::isKeyPressed(GLFW_KEY_S)) std::cout << "pressed S\n";
	if (Input::isKeyPressed(GLFW_KEY_D)) std::cout << "pressed D\n";
}

void InputHandler::mouseHandle(float deltaTime)
{
	if (Input::isButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) std::cout << "pressed LEFT\t";
	if (Input::isButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) std::cout << "pressed RIGHT\t";
}
