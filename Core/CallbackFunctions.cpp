#include "CallbackFunctions.h"

bool Input::m_keys[1024] = { false };
bool Input::m_mouseButtons[8] = { false };

void Input::initInput(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

bool Input::isKeyPressed(int key)
{
	return m_keys[key];
}

bool Input::isButtonPressed(int button)
{
	return m_mouseButtons[button];
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) m_keys[key] = true;
	else if (action == GLFW_RELEASE) m_keys[key] = false;
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) m_mouseButtons[button] = true;
	else if (action == GLFW_RELEASE) m_mouseButtons[button] = false;
}