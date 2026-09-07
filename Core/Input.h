#pragma once
#include "Gui.h"

//
enum KeyAction {
	RELEASED = -1,
	NOTHING = 0,
	PRESSED = 1
} ;

struct MousePos
{
	double x = 0.0f; 
	double y = 0.0f;
};

class Input
{
public:
	Input() {}
	~Input() {}

	static void initInput(GLFWwindow* window);

	static bool isKeyPressed(int key);
	static bool isKeyReleased(int key);
	static bool isKeyHeld(int key);
	static bool isButtonPressed(int button);
	static MousePos getMousePos();

	static float m_fov;
private:

	static MousePos m_mousePos;

	static KeyAction m_keyAction[1024];
	static KeyAction m_mouseButtons[8];


	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};