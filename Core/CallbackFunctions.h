#pragma once
#include <GLFW/glfw3.h>

//
class Input
{
public:
	Input() {}
	~Input() {}

	static void initInput(GLFWwindow* window);

	static bool isKeyPressed(int key);
	static bool isButtonPressed(int button);
	static bool m_keys[1024];
	static bool m_mouseButtons[8];

private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};