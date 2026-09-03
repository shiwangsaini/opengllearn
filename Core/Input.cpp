#include "Input.h"

#include <iostream>

KeyAction Input::m_keyAction[1024] = { KeyAction::NOTHING };
KeyAction Input::m_mouseButtons[8] = { KeyAction::NOTHING };
MousePos Input::m_mousePos = { 0.0f,0.0f };

void Input::initInput(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
}

bool Input::isKeyPressed(int key)
{
	if (m_keyAction[key] == KeyAction::PRESSED) {
		m_keyAction[key] = KeyAction::NOTHING;
		return true;
	}
	return false;
}

bool Input::isKeyReleased(int key)
{
	if (m_keyAction[key] == KeyAction::RELEASED) {
		m_keyAction[key] = KeyAction::NOTHING;
		return true;
	}
	return false;
}

bool Input::isKeyHeld(int key)
{
	return m_keyAction[key] == KeyAction::PRESSED;
}

bool Input::isButtonPressed(int button)
{
	return m_mouseButtons[button] == KeyAction::PRESSED;
}

MousePos Input::getMousePos()
{
	return m_mousePos;
}


#pragma region CallBacks
void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
	if (action == GLFW_PRESS) m_keyAction[key] = KeyAction::PRESSED;
	else if (action == GLFW_RELEASE) {
		m_keyAction[key] = KeyAction::RELEASED;
	}
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	if (action == GLFW_PRESS) m_mouseButtons[button] = KeyAction::PRESSED;
	else if (action == GLFW_RELEASE) m_mouseButtons[button] = KeyAction::RELEASED;
}

void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

	m_mousePos.x = xpos;
	m_mousePos.y = ypos;
}

#pragma endregion